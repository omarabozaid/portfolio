from flask import Flask, render_template, redirect, url_for, request, session, flash
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager, UserMixin, login_user, login_required, logout_user, current_user

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your_secret_key'
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///site.db'
db = SQLAlchemy(app)
login_manager = LoginManager(app)

# Models
class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(20), unique=True, nullable=False)
    password = db.Column(db.String(60), nullable=False)
    carts = db.relationship('Cart', backref='user', lazy=True)

class Product(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(50), nullable=False)
    price = db.Column(db.Float, nullable=False)

class Cart(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)
    product_id = db.Column(db.Integer, db.ForeignKey('product.id'), nullable=False)

class PurchasedItem(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)
    product_id = db.Column(db.Integer, db.ForeignKey('product.id'), nullable=False)

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

@app.route('/')
def index():
    if current_user.is_authenticated:
        products = Product.query.all()
        return render_template('index.html', products=products)
    else:
        return redirect(url_for('signin'))
    
@app.route('/product/<int:product_id>')
def product(product_id):
    product = Product.query.get(product_id)
    return render_template('product.html', product=product)

@app.route('/add_to_cart/<int:product_id>')
@login_required
def add_to_cart(product_id):
    product = Product.query.get(product_id)
    
    if product:
        cart = Cart(user=current_user, product_id=product.id)
        db.session.add(cart)
        db.session.commit()
        flash('Item added to cart', 'success')
    else:
        flash('Product not found', 'error')

    return redirect(url_for('index'))

@app.route('/cart')
@login_required
def view_cart():
    user_cart = Cart.query.filter_by(user=current_user).all()
    return render_template('cart.html', cart=user_cart)

@app.route('/checkout')
@login_required
def checkout():
    user_cart = Cart.query.filter_by(user=current_user).all()
    for cart_item in user_cart:
        purchased_item = PurchasedItem(user=current_user, product=cart_item.product)
        db.session.add(purchased_item)
        db.session.delete(cart_item)
    db.session.commit()
    flash('Items purchased successfully', 'success')
    return redirect(url_for('index'))

@app.route('/signin', methods=['GET', 'POST'])
def signin():
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')

        # Replace the following logic with your actual user authentication logic
        user = User.query.filter_by(username=username, password=password).first()

        if user:
            login_user(user)
            flash('Logged in successfully', 'success')
            return redirect(url_for('index'))
        else:
            flash('Invalid username or password', 'error')

    return render_template('signin.html')

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        confirm_password = request.form.get('confirm_password')

        # Replace the following logic with your actual user registration logic
        if password == confirm_password:
            # Check if the username is available (not already taken)
            existing_user = User.query.filter_by(username=username).first()
            if existing_user:
                flash('Username already exists. Please choose another.', 'error')
            else:
                # Create a new user
                new_user = User(username=username, password=password)
                db.session.add(new_user)
                db.session.commit()

                flash('Account created successfully. Please sign in.', 'success')
                return redirect(url_for('signin'))
        else:
            flash('Passwords do not match. Please try again.', 'error')

    return render_template('signup.html')

@app.route('/logout')
@login_required
def logout():
    logout_user()
    flash('Logged out successfully', 'success')
    return redirect(url_for('index'))

# Run the app
if __name__ == '__main__':
    with app.app_context():
        db.create_all()
        if not Product.query.all():
            dummy_products = [
                {"name": "Laptop", "price": 1200.0},
                {"name": "Smartphone", "price": 800.0},
                {"name": "Headphones", "price": 150.0},
                {"name": "Camera", "price": 1000.0},
                {"name": "Printer", "price": 300.0},
                {"name": "Tablet", "price": 500.0},
            ]
            for product_data in dummy_products:
                product = Product(name=product_data["name"], price=product_data["price"])
                db.session.add(product)
            db.session.commit()
    app.run(debug=True)