from typing import List

class Node:
    def __init__(self,value:int=0,left=None,right=None)->None:
        self.value = value
        self.left =left
        self.right = right
        
    def __lt__(self, other_node: 'Node') -> bool:
        return self.value < other_node.value

    def __gt__(self, other_node: 'Node') -> bool:
        return self.value > other_node.value

    def __eq__(self, other_node: 'Node') -> bool:
        if other_node is None or not isinstance(other_node, Node):
            return False
        return self.value == other_node.value

class BST:
    def __init__(self,value)->None:
        self.root:'Node'=Node(value)
        self.not_found:'Node'=Node(-1e100)
        
    def build_tree(self,values:List[int]):
        for value in values:
            self.insert(self.root,Node(value))
    
    def insert(self,parent_node:'Node',child_node:'Node')->None:
        if child_node == parent_node:
            pass
        if child_node<parent_node:
            if parent_node.left == None:
                parent_node.left = child_node
            else:
                self.insert(parent_node=parent_node.left,child_node=child_node)
        else:
            if parent_node.right == None:
                parent_node.right = child_node
            else:
                self.insert(parent_node=parent_node.right,child_node=child_node)
                
    def search(self,value:int)->'Node':
        searchnode:'Node'=Node(value)
        return self.recursive_search(parent_node=self.root,othernode=searchnode)
    
    def recursive_search(self,parent_node:'Node',othernode:'Node')->'Node':
        if parent_node == othernode:
            return parent_node
        elif parent_node<othernode and parent_node.left != None:
            return self.recursive_search(parent_node=parent_node.left,othernode=othernode) 
        elif parent_node>othernode and parent_node.right != None:
            return self.recursive_search(parent_node=parent_node.right,othernode=othernode) 
        else:
            return self.not_found
        