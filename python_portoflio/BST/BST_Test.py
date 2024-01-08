import unittest
from typing import List
from BST import BST

class Test_BST(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        print('Start BST Test')

    @classmethod
    def tearDownClass(cls) -> None:
        print('Done BST Test')
        
    def setUp(self) -> None:
        self.values:List[int] = [-10,5,3,8,11,-17,-30,-2]
        self.root:int=3
        self.bst = BST(value=self.root)
        
    def tearDown(self) -> None:
        self.values=None
        self.root=None
        
    def test_build_bst(self):
        self.bst.build_tree(values=self.values)
        self.traverse_bst(current_node=self.bst.root)
        
    def traverse_bst(self,current_node):
        left_node  = current_node.left
        right_node = current_node.right
        if left_node != None:
            self.assertLess(left_node.value,current_node.value)
            self.traverse_bst(left_node)
        if right_node != None:
            self.assertGreater(right_node.value,current_node.value)
            self.traverse_bst(right_node)