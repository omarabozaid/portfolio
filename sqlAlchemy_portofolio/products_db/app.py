# app.py

from sqlalchemy import create_engine, Column, Integer, String, ForeignKey, Float
from sqlalchemy.orm import declarative_base, relationship
import pytest
import random

Base = declarative_base()

class Product(Base):
    __tablename__ = 'products'
    id = Column(Integer, primary_key=True)
    name = Column(String(50), nullable=False)
    price = Column(Float, nullable=False)
    manufacturer_id = Column(Integer, ForeignKey('manufacturers.id'))
    seller_id = Column(Integer, ForeignKey('sellers.id'))

    manufacturer = relationship('Manufacturer', back_populates='products')
    seller = relationship('Seller', back_populates='products')

class Manufacturer(Base):
    __tablename__ = 'manufacturers'
    id = Column(Integer, primary_key=True)
    name = Column(String(50), nullable=False)
    products = relationship('Product', back_populates='manufacturer')

class Seller(Base):
    __tablename__ = 'sellers'
    id = Column(Integer, primary_key=True)
    name = Column(String(50), nullable=False)
    products = relationship('Product', back_populates='seller')


class Client(Base):
    __tablename__ = 'clients'
    id = Column(Integer, primary_key=True)
    name = Column(String(50), nullable=False)

def create_database():
    engine = create_engine('sqlite:///products.db')
    Base.metadata.create_all(engine)
    return engine

# Create an instance of the SQLAlchemy engine
db_engine = create_database()

# Create a session to interact with the database
from sqlalchemy.orm import sessionmaker
Session = sessionmaker(bind=db_engine)
session = Session()

# Add some sample data
manufacturers = [Manufacturer(name=f'Manufacturer {i}') for i in range(1, 4)]
sellers = [Seller(name=f'Seller {i}') for i in range(1, 4)]

for m in manufacturers:
    session.add(m)

for s in sellers:
    session.add(s)

session.commit()

for i in range(1, 11):
    product = Product(
        name=f'Product {i}',
        price=random.randint(10, 100),
        manufacturer=random.choice(manufacturers),
        seller=random.choice(sellers)
    )
    session.add(product)

session.commit()

# Close the session after adding data
session.close()
