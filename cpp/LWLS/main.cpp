/*
    Light weight header single header file serial implementation of basic set of 
    Linear solvers and preconditioners for a sparse matrix
    Author: Dr.-Ing. Omar ELSAYED
    License: only for EDUCATIONAL purposes otherwise contact the author
    E-mail: omarabozaid@aucegypt.edu
    MPI parallelisation: under-developement
    Date: 30.06.2023
    Version: 0
    Lib-name: Light Weight Linear Solver LWLS
*/

#include "LWLS.hpp"
#include <random>
#include <iostream>

void license() {
    std::cout << "/*\n"
                 "    Light weight header single header file serial implementation of basic set of \n"
                 "    Linear solvers and preconditioners for a sparse matrix\n"
                 "    Author: Dr.-Ing. Omar ELSAYED\n"
                 "    License: only for EDUCATIONAL purposes otherwise contact the author\n"
                 "    E-mail: omarabozaid@aucegypt.edu\n"
                 "    MPI parallelisation: under-developement\n"
                 "    Date: 30.06.2023\n"
                 "    Version: 0\n"
                 "    Lib-name: Light Weight Linear Solver LWLS\n"
                 "*/" << std::endl;
}

void test(){
    LWLS::Label n = 5000;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    
    LWLS::VectorOfDoubles diag(n,1);
    LWLS::VectorOfDoubles sym_off_diag(diag.size()-1,0);


    LWLS::dense_vector dv(LWLS::VectorOfDoubles(diag.size(),0));
    for(int i = 0; i<diag.size(); i++){
        dv(i) = dis(gen);
    }

    LWLS::CartesianVector non_zero_indices(diag.size()+2*sym_off_diag.size());
    for(int i = 0; i<diag.size(); i++){
        non_zero_indices[i] = LWLS::ij(i,i);
    }
    for(int i = diag.size(); i<(diag.size()+sym_off_diag.size()); i++){
        auto row = i- diag.size();
        auto col = row+1;
        non_zero_indices[i].set_row_col(row,col);
    }
    
    for(
        int i = diag.size()+sym_off_diag.size(); 
        i<(diag.size()+2*sym_off_diag.size()); 
        i++
    ){
        auto row = i- (diag.size()+sym_off_diag.size())+1;
        auto col = row-1;
        non_zero_indices[i].set_row_col(row,col);
    }
    
    LWLS::VectorOfDoubles non_zero_values(non_zero_indices.size(),0.0);
    for(int i = 0; i<diag.size(); i++){
        non_zero_values[i] = 4.0+dis(gen);
    }
    for(int i = diag.size(); i<(diag.size()+sym_off_diag.size()); i++){
        non_zero_values[i] = -dis(gen);
    }
    for(
        int i = diag.size()+sym_off_diag.size(); 
        i<(diag.size()+2*sym_off_diag.size()); 
        i++
    ){
        non_zero_values[i] = non_zero_values[i-sym_off_diag.size()];
    }

    LWLS::sparse_matrix Mat(non_zero_indices,non_zero_values,n,n);

    LWLS::conjugate_gradient cg_solver(Mat,dv);
    auto cg_solution = cg_solver.solve();


    LWLS::gauss_seidel gs_solver(Mat,dv);
    auto gs_solution = gs_solver.solve();

    LWLS::biconjugate_gradient bg_solver(Mat,dv);
    auto bg_solution = bg_solver.solve();

    LWLS::stabilized_biconjugate_gradient sbg_solver(Mat,dv);
    auto sbg_solution = sbg_solver.solve();

    LWLS::successive_over_relaxation sov_solver(Mat,dv);
    auto sov_solution = sov_solver.solve();

    /*
    std::cout<<cg_solution<<"\n";   
    std::cout<<gs_solution<<"\n";
    std::cout<<bg_solution<<"\n";
    std::cout<<sbg_solution<<"\n";
    std::cout<<sov_solution<<"\n";
    */
}

int main(){
    license();
    test();
    return 0;
}