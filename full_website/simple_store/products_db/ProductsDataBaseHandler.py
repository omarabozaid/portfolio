import sys
import os
rel_root_dir = os.path.dirname(os.path.relpath(__file__))
sys.path.append(rel_root_dir)

from flask import Flask, jsonify

from ProductsDataBase import ProductsDataBase

class ProductsDataBaseHandler:
    def __init__(self):
        self.ProductDB_Object=ProductsDataBase()
      
    def get_product(self,country,category=None,product_name=None):
        products_query = self.ProductDB_Object.query_products(
            country_name=country,category=category,product_name=product_name
        )
        p = [
            {
                'product_name': p.name,
                'product_company': p.company.name,
                'product_id': p.id
            } 
            for p in products_query
        ]
        return p