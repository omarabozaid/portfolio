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

# Routes
@app.route('/')
def index():
    products = Product.query.all()
    return render_template('index.html', products=products)

@app.route('/product/<int:product_id>')
def product(product_id):
    product = Product.query.get(product_id)
    return render_template('product.html', product=product)

@app.route('/add_to_cart/<int:product_id>')
@login_required
def add_to_cart(product_id):
    product = Product.query.get(product_id)
    cart = Cart(user=current_user, product=product)
    db.session.add(cart)
    db.session.commit()
    flash('Item added to cart', 'success')
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

# Run the app
if __name__ == '__main__':
    with app.app_context():
        db.create_all()
        # Add some dummy products if the Product table is empty
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