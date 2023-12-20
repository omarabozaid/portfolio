from typing import List, Optional
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

'''
    ----> Recursive construction of an octree 
    ----> Plot of an octree
    ----> Contact: omarabozaid@aucegypt.edu
    ----> Author:  Dr.-Ing. Omar ELSAYED
'''

class Box:
    def __init__(self, Center: np.ndarray, Dx: float, Level: int = 0) -> None:
        self.Level: int = Level
        self.Center: np.ndarray = Center
        self.Dx: float = Dx
        self.Children_Indices: List[Optional[int]] = [None] * 8

    def Set_Child(self, Another_Box: int, Indicator: int) -> None:
        self.Children_Indices[Indicator] = Another_Box

    def Get_child(self, Indicator: int) -> Optional[int]:
        return self.Children_Indices[Indicator]

class PointCloud:
    def __init__(self, Points: List[np.ndarray]) -> None:
        self.Points = Points
        self.Update_Center()

    def Update_Center(self) -> None:
        if len(self.Points) > 0:
            self.Center = np.mean(self.Points, axis=0)

    def Get_Center(self) -> np.ndarray:
        return self.Center
    
    def Get_BB(self):
        Min = np.min(self.Points, axis=0)
        Max = np.max(self.Points, axis=0)
        DMax = Max - self.Center
        DMin = Min - self.Center
        Dx = np.max(np.maximum(DMax, -DMin))
        return DMax, DMin, Dx

BoxRays = [
    [1,     1,  1],
    [1,    -1,  1],
    [-1,   -1,  1],
    [-1,    1,  1],
    [1,     1,  -1],
    [1,    -1,  -1],
    [-1,   -1,  -1],
    [-1,    1,  -1]
]

class Octree:
    def __init__(self, Points: List[np.ndarray], Constraint: int, MaxDepth: int = 3) -> None:
        self.Cloud: PointCloud = PointCloud(Points=Points)
        DMax, DMin, Dx = self.Cloud.Get_BB()
        self.Boxes: List[Box] = [Box(Center=self.Cloud.Get_Center(), Dx=Dx, Level=0)]
        self.Constraint: int = Constraint
        self.MaxDepth:int = MaxDepth
        self.Build(self.Boxes[0], self.Cloud)

    def Build(self, ParentBox: 'Box', Cloud: 'PointCloud') -> None:
        if len(Cloud.Points) > self.Constraint and ParentBox.Level < self.MaxDepth:
            Dx = ParentBox.Dx
            for i in range(8):
                ChildCenter = ParentBox.Center + (0.5 * Dx * np.asarray(BoxRays[i]))
                SubCloud = self.Get_SubCloud(Center=ChildCenter, Dx=0.5*Dx, Cloud=Cloud)
                ChildBox = Box(Center=ChildCenter, Dx=(0.5 * Dx), Level=ParentBox.Level + 1)
                ParentBox.Set_Child(len(self.Boxes), i)
                self.Boxes.append(ChildBox)
                self.Build(ChildBox, SubCloud)

    def Get_SubCloud(self, Center: np.ndarray, Dx: float, Cloud: 'PointCloud') -> 'PointCloud':
        PointsInRegion = [P for P in Cloud.Points if np.all((Center - P) ** 2 <= Dx ** 2)]
        return PointCloud(Points=PointsInRegion)

def visualize_octree(octree: Octree):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    for box in octree.Boxes:
        plot_box(ax,box)

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.title('Octree')
    plt.show()

def plot_box(ax, box: Box):
    d = box.Dx
    center = box.Center
    vertices = np.array([center+d*np.asarray(BoxRays[i]) for i in range(8)])
    faces = [
        [vertices[0], vertices[1], vertices[2], vertices[3]],
        [vertices[4], vertices[5], vertices[6], vertices[7]],
        [vertices[0], vertices[1], vertices[5], vertices[4]],
        [vertices[2], vertices[3], vertices[7], vertices[6]],
        [vertices[0], vertices[3], vertices[7], vertices[4]],
        [vertices[1], vertices[2], vertices[6], vertices[5]],
        [vertices[0], vertices[3], vertices[2], vertices[1]],
    ]
    for face in faces:
        face = np.array(face)
        ax.plot3D(face[:, 0], face[:, 1], face[:, 2], color='r')

def generate_random_points(n):
    points = np.random.uniform(low=[0, 0, 0], high=[1, 1, 1], size=(n, 3))
    return points

def main():
    n:int = 300
    Points = generate_random_points(n)
    tree: Octree = Octree(Points=Points, Constraint=18, MaxDepth=20)
    visualize_octree(octree=tree)
    
if __name__ == '__main__':
    main()
