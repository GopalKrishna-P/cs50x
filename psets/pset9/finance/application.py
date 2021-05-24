import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
# export API_KEY=pk_5aa41bce6c33448685297a911352cbd9
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # get username of the current
    username = db.execute("SELECT username FROM users WHERE id = (?)", int(session['user_id']))[0]["username"]

    # portfolio
    portfolio = db.execute("SELECT * FROM portfolio WHERE username=(?)", username)

    # cash
    cash = db.execute("SELECT cash FROM users WHERE username=(?)", username)[0]["cash"]

    # total & update the prices based on value from API
    total = float(cash)
    for stock in portfolio:
        curr_value = lookup(str(stock["symbol"]))
        stock["price"] = usd(curr_value["price"])

        curr_total = int(stock["shares"]) * curr_value["price"]
        stock["total"] = usd(curr_total)
        total += curr_total

    return render_template("index.html", cash=cash, portfolio=portfolio, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # symbol and no of shares from user input
        look = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")

        # validate the symbol and no of shares
        if look == None:
            return apology("Invalid symbol", 400)
        if not shares.isdigit() or int(shares) < 1:
            return apology("Invalid shares", 400)

        # cash available for the current user
        cash = db.execute("SELECT cash FROM users WHERE id = (?)", int(session['user_id']))

        # required amount for the purchase
        value = look["price"] * int(shares)

        # Validating the transcation
        if int(cash[0]["cash"]) < value:
            # insuffient funds
            return apology("Insuficient cash to purchase", 403)
        else:
            # update the cash for the user
            db.execute("UPDATE users SET cash = cash - (?) WHERE id = (?)", value, int(session['user_id']))

            # Add to transcation table
            db.execute("INSERT INTO history (username, operation, symbol, price, shares) VALUES (:username, 'BUY', :symbol, :price, :shares)",
                       username=db.execute("SELECT username FROM users WHERE id = :user_id",
                                           user_id=int(session['user_id']))[0]["username"],
                       symbol=look['symbol'], price=look['price'], shares=request.form.get('shares'))

            # Add stocks to user's portfolio
            db.execute("INSERT INTO portfolio (username, symbol, shares) VALUES (:username, :symbol, :shares)",
                       username=db.execute("SELECT username FROM users WHERE id = :user_id",
                                           user_id=int(session['user_id']))[0]["username"],
                       symbol=look['symbol'], shares=request.form.get('shares'))

            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    username = db.execute("SELECT username FROM users WHERE id = :uid", uid=int(session['user_id']))[0]["username"]

    stocks = db.execute(
        "SELECT operation, symbol, price, date, time, shares FROM history WHERE username = :username", username=username)

    for stock in stocks:
        symbol = str(stock["symbol"])
        name = lookup(symbol)["name"]
        stock["name"] = name

    return render_template("history.html", stocks=stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == 'GET':
        return render_template("quote.html", stock=None)
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("no symbol entered")

        stock = lookup(symbol)
        if not stock:
            return apology("Invalid symbol")

        return render_template("quote.html", stock=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # get user input
    username = request.form.get("username")
    password = request.form.get("password")

    # method call
    if request.method == "GET":
        return render_template("register.html")
    else:
        # validating the username
        if not username:
            return apology("must provide username", 400)
        elif len(db.execute("SELECT * FROM users WHERE username=(?) LIMIT 1", username)) == 1:
            return apology("username is already taken", 400)

        # validate password
        if not password or not request.form.get("confirmation"):
            return apology("must provide password", 400)
        if password != request.form.get("confirmation"):
            return apology("passwords must match", 400)

        # insert into the users .db
        if not db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=generate_password_hash(password)):
            return apology("Invalid credentials, try again", 400)

        # log in
        session["user_id"] = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    username = db.execute("SELECT username FROM users WHERE id = :uid", uid=int(session['user_id']))[0]["username"]

    if request.method == "GET":
        # list all the available stocks user can sell
        symbols = db.execute("SELECT symbol FROM portfolio WHERE username = (?)", username)
        return render_template("sell.html", symbols=symbols)
    else:
        # stock and share user selected to sell
        stock = lookup(request.form.get("symbol"))
        shares = request.form.get("shares")

        # save total shares
        user_shares = db.execute("SELECT shares FROM portfolio WHERE username = :username and symbol = :symbol",
                                 username=username, symbol=str(request.form.get("symbol")))[0]["shares"]

        # save values of shares
        value = stock["price"] * int(shares)

        # apologies
        if not request.form.get("symbol") or stock == None:
            return apology("Invalid stock", 400)
        elif not shares or not shares.isdigit() or int(shares) < 1 or int(shares) > int(user_shares):
            return apology("Invalid no. of shares", 400)

        # start selling
        else:
            # valuing
            db.execute("UPDATE users SET cash = cash + :value WHERE id = :user_id", value=value, user_id=int(session['user_id']))

            # adding to history
            db.execute("INSERT INTO history (username, operation, symbol, price, shares) VALUES (:username, 'SELL', :symbol, :price, :shares)",
                       username=username, symbol=stock['symbol'], price=stock['price'], shares=request.form.get('shares'))

            # removing (if all sold)
            if int(user_shares) == int(shares):
                db.execute("DELETE FROM portfolio WHERE username = :username and symbol = :symbol",
                           username=username, symbol=str(request.form.get("symbol")))

            # updating (if not all sold)
            elif int(user_shares) > int(shares):
                db.execute("UPDATE portfolio SET shares = :shares WHERE username = :username and symbol = :symbol",
                           shares=int(user_shares) - int(shares), username=username, symbol=request.form.get("symbol"))

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
