import sys
import os
from sqlalchemy import create_engine, ForeignKey, Column, Integer, String, Float, Boolean, Date, or_, and_
from sqlalchemy.orm import relationship, sessionmaker, declarative_base
from datetime import datetime

rel_root_dir = os.path.dirname(os.path.relpath(__file__))
sys.path.append(rel_root_dir)

Base = declarative_base()

class Country(Base):
    __tablename__ = 'countries'
    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(50), nullable=False, unique=True)
    
    products = relationship('Product', back_populates='country')
    companies = relationship('Company', back_populates='country')

class Company(Base):
    __tablename__ = 'companies'
    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(50), nullable=False)
    country_id = Column(Integer, ForeignKey('countries.id'), nullable=False)

    country = relationship('Country', back_populates='companies')
    products = relationship('Product', back_populates='company')

class Product(Base):
    __tablename__ = 'products'
    id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(50), nullable=False)
    category = Column(String(50), nullable=False)
    price = Column(Float, nullable=False)
    quantity_in_stock = Column(Integer, nullable=False)
    company_id = Column(Integer, ForeignKey('companies.id'), nullable=False)
    country_id = Column(Integer, ForeignKey('countries.id'), nullable=False)

    company = relationship('Company', back_populates='products')
    country = relationship('Country', back_populates='products')

    def __repr__(self):
        return f"{self.name} by {self.company.name} in {self.country.name}"

class ProductsDataBase:
    def __init__(self):
        db_dir = os.path.abspath(os.path.join(rel_root_dir,'..'))
        db_name = 'products.db'
        instance_folder = os.path.join(db_dir, 'instance')
        if not os.path.exists(instance_folder):
            os.makedirs(instance_folder)
        db_path = os.path.join(instance_folder, db_name)
        self.engine = create_engine(f'sqlite:///{db_path}')        
        Base.metadata.create_all(self.engine)
        Session = sessionmaker(bind=self.engine)
        self.products_db_session = Session()
        
    def session(self):
        return self.products_db_session

    def query_products(self, country_name=None, category=None, product_name=None):
        query = self.products_db_session.query(Product)
        filters = []
        if country_name:
            filters.append(Country.name == country_name)
        if category:
            filters.append(Product.category == category)
        if product_name:
            filters.append(Product.name == product_name)
        if filters:
            query = query.join(Country).filter(or_(*filters))
        return query.all()
    
def random_filling():
    db_dir = os.path.abspath(os.path.join(rel_root_dir,'..'))
    instance_folder = os.path.join(db_dir, 'instance')
    db_name = 'products.db'
    db_path = os.path.join(instance_folder, db_name)
    engine = create_engine(f'sqlite:///{db_path}')        
    Base.metadata.create_all(bind=engine)
    Session = sessionmaker(bind=engine)
    session = Session()
    
    country1 = Country(name="Egypt")
    country2 = Country(name="Tunisia")
    
    company1 = Company(name="Company", country=country1)
    company2 = Company(name="AnotherCompany", country=country2)
    
    product1 = Product(
        name="TV", category="Electronics", 
        price=100.0, quantity_in_stock=50, 
        company=company1, country=country1
    )
    
    product2 = Product(
        name="Laptop", category="Electronics", 
        price=1000.0, quantity_in_stock=50, 
        company=company1, country=country1
    )
    
    product3 = Product(
        name="Chair", category="Electronics", 
        price=10.0, quantity_in_stock=50, 
        company=company1, country=country1
    )
    
    product4 = Product(
        name="Story", category="Electronics", 
        price=2.0, quantity_in_stock=50, 
        company=company1, country=country1
    )
    
    product5 = Product(
        name="Product2", category="Clothing", 
        price=50.0, quantity_in_stock=100,
        company=company2, country=country1
    )
    
    product6 = Product(
        name="Camera", category="Electronics", 
        price=20.0, quantity_in_stock=50, 
        company=company1, country=country2
    )
    session.add_all([country1, country2, company1, company2, product1, product2,product3,product4,product5,product6])
    session.commit()
    


#random_filling()