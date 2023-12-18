/*****************************************************
 * Robuts Modeling Zenith ----   CFD                   
 * Author: Dr.-Ing. Omar ELSAYED                       
 * Date of creation of file: 2023-12-14
 * RMZ_CFD                                 
 *                                                     
 ******************************************************/

#ifndef LWLS_HPP
#define LWLS_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>

namespace LWLS{
    class ij;
    class row_or_col;
    class vector;
    class dense_vector;
    class sparse_vector;
    class sparse_matrix;

    using Label =                 unsigned int;
    using Double =                double;
    using VectorOfLabels=         std::vector<Label>;
    using VectorOfDoubles=        std::vector<Double>;
    using LabelLabelMap=          std::unordered_map<Label,Label>;
    using CartesianVector=        std::vector<ij>;
    using LabelLabelPair=         std::pair<Label,Label>;
    using LabelDoublePair=        std::pair<Label,Double>;
    using RowSpace=               std::vector<row_or_col>;
    using ColSpace=               std::vector<row_or_col>;

    class ij{
        public:
            ij(Label i=0, Label j=0):i_(i),j_(j){};
            const Label& row()  {return i_;};
            const Label& col()  {return j_;};
            const Label& row()const{return i_ ;};
            const Label& col()const{return j_ ;};
            void set_row(Label i){i_ = i;};
            void set_col(Label j){j_ = j;};
            void set_row_col(Label i, Label j){ set_row(i); set_col(j);};
            void T(){std::swap(i_,j_);};

            bool operator == (const ij& row_col) {
                if(i_==row_col.i_ && j_==row_col.j_){
                    return true;
                }
                return false;
            }
            
            bool operator == (const ij& row_col) const {
                if(i_==row_col.i_ && j_==row_col.j_){
                    return true;
                }
                return false;
            }
            
            bool operator<(const ij& row_col_){
                if(i_<row_col_.i_ && j_<row_col_.j_){
                    return true;
                }
                return false;
            }

            bool operator < (const ij& row_col_) const {
                if(i_<row_col_.i_ && j_<row_col_.j_){
                    return true;
                }
                return false;
            }
        private:
            Label i_,j_ ;
    };

    class vector{
        public:
            vector(){};
            vector(const vector& v){};
            virtual const Double& operator [] (const Label& index) const = 0;
            virtual const Double& operator [] (const Label& index)  = 0;
            const Double& zero() const { 
                return zero_;
            };
            const Double& zero()  {
                return zero_;
            };
        private:
            static Double zero_ ;

    };
    Double vector::zero_ = 0.0;

    class dense_vector: vector{
        public:
            dense_vector(VectorOfDoubles values):vector(),values_(values){};
            dense_vector(Label size, Double uniform_value=0.0):vector(),values_(VectorOfDoubles(size,uniform_value)){};

            VectorOfDoubles::iterator begin() { return values_.begin(); }
            VectorOfDoubles::iterator end() { return values_.end(); }
            VectorOfDoubles::const_iterator begin() const { return values_.begin(); }
            VectorOfDoubles::const_iterator end() const { return values_.end(); }


            const Double& operator [] (const Label& index){
                if (index<=values_.size())
                    {
                        return values_[index];
                    }
                else
                    {
                        std::cout<<"error:accessing non existing element-return zero\n";
                        return vector::zero();
                    }
            };
            const Double& operator [] (const Label& index) const{
                return const_cast<dense_vector*>(this)->operator[](index) ;
            };
            
            Double& operator() (const Label& index){
                return values_[index];
            }

            void resize_with_value(Label size, Label uniform_value){
                values_ = VectorOfDoubles(size,uniform_value);
            }
            
            friend std::ostream& operator << (std::ostream& os, const dense_vector& v){
                Label v_size_ = v.values_.size();
                Label iter_ = 0 ;
                for(const auto& value_:v.values_){
                    os<<value_;
                    iter_+=1;
                    if(iter_<v_size_){
                        os<<"\n";
                    }
                }
                os<<"\n";
                return os;
            } 
            
            Double size()       {return values_.size();};
            Double size() const {return values_.size();};
            
            Double operator & (const dense_vector& v) {
                Double inner_product_ = 0.0 ;
                Label iter_ = 0 ;
                for(const auto& value_ : values_){
                    inner_product_+=(value_*v[iter_]);
                    iter_+=1;
                }
                return inner_product_;
            };

            Double operator & (const dense_vector& v) const {
                Double inner_product_ = 0.0 ;
                Label iter_ = 0 ;
                for(const auto& value_ : values_){
                    inner_product_+=(value_*v[iter_]);
                    iter_+=1;
                }
                return inner_product_;
            };

            Double norm() {
                return std::sqrt(this->operator&(*this));
            };

            Double norm() const{
                return std::sqrt(this->operator&(*this));
            };

            Double sum() {
                Double sum_ = 0 ;
                for(const auto& val:values_){
                    sum_+=val;
                }
                return sum_;
            };

            Double sum() const{
                Double sum_ = 0 ;
                for(const auto& val:values_){
                    sum_+=val;
                }
                return sum_;
            };

            Double sum_absolutes()  {
                Double sum_ = 0 ;
                for(const auto& val:values_){
                    sum_+=std::abs(val);
                }
                return sum_;
            };

            Double sum_absolutes() const{
                Double sum_ = 0 ;
                for(const auto& val:values_){
                    sum_+=std::abs(val);
                }
                return sum_;
            };

            dense_vector operator*(const Double& lambda) {
                VectorOfDoubles scaled_values_(values_);
                for(auto& value_:scaled_values_){
                    value_*=lambda;
                };
                return dense_vector(scaled_values_);
            };

            dense_vector operator*(const Double& lambda) const {
                VectorOfDoubles scaled_values_(values_);
                for(auto& value_:scaled_values_){
                    value_*=lambda;
                };
                return dense_vector(scaled_values_);
            };

            dense_vector operator/(const Double& lambda)  {
                if(std::abs(lambda) < 1e-14){
                    std::cout<<"warning division by almost zero \n";
                    Double stable_lambda_ = 1e-14;
                    return this->operator*(1/stable_lambda_);
                }
                else{
                    return this->operator*(1/lambda);
                }
            };

            friend dense_vector operator * (const Double& lambda, const dense_vector& v){
                return v*lambda;
            };

            void operator += (const dense_vector& add_v){
                const auto& add_values_ = add_v.values_;
                Label iter_ = 0 ;
                for(auto& value_ : values_){
                    value_+=add_values_[iter_];
                    iter_+=1;
                }
            };

            void operator -= (const dense_vector& add_v){
                const auto& add_values_ = add_v.values_;
                Label iter_ = 0 ;
                for(auto& value_ : values_){
                    value_-=add_values_[iter_];
                    iter_+=1;
                }
            };

            dense_vector operator + (const dense_vector& add_v){
                const auto& add_values_ = add_v.values_;
                VectorOfDoubles new_values_(values_.size(),0);
                Label iter_ = 0 ;
                for(auto& value_ : new_values_){
                    value_ = add_values_[iter_]+values_[iter_];
                    iter_+=1;
                }
                return dense_vector(new_values_);
            };

            dense_vector operator + (const dense_vector& add_v) const {
                const auto& add_values_ = add_v.values_;
                VectorOfDoubles new_values_(values_.size(),0);
                Label iter_ = 0 ;
                for(auto& value_ : new_values_){
                    value_ = add_values_[iter_]+values_[iter_];
                    iter_+=1;
                }
                return dense_vector(new_values_);
            };

            dense_vector operator - (const dense_vector& add_v){
                const auto& add_values_ = add_v.values_;
                VectorOfDoubles new_values_(values_.size(),0);
                Label iter_ = 0 ;
                for(auto& value_ : new_values_){
                    value_ = -add_values_[iter_]+values_[iter_];
                    iter_+=1;
                }
                return dense_vector(new_values_);
            };

            dense_vector operator - (const dense_vector& add_v) const {
                const auto& add_values_ = add_v.values_;
                VectorOfDoubles new_values_(values_.size(),0);
                Label iter_ = 0 ;
                for(auto& value_ : new_values_){
                    value_ = -add_values_[iter_]+values_[iter_];
                    iter_+=1;
                }
                return dense_vector(new_values_);
            };

        private:
            VectorOfDoubles values_;
            
    };

    class sparse_vector: vector{
    public:
        sparse_vector(
            VectorOfLabels elements_indices, VectorOfDoubles nonzero_elements)
            :vector(),
            elements_indices_(elements_indices),
            nonzero_elements_(nonzero_elements),
            size_(0)
        {
            Label key_ = 0 ;
            for(const auto& index_:elements_indices_){
                elements_hashmap_.insert(LabelLabelPair(index_,key_));
                key_+=1;
            }
            this->update_size_();
        };

        sparse_vector(
            VectorOfLabels elements_indices, VectorOfDoubles nonzero_elements,Label size)
            :vector(),
            elements_indices_(elements_indices),
            nonzero_elements_(nonzero_elements),
            size_(size)
        {
            Label key_ = 0 ;
            for(const auto& index_:elements_indices_){
                elements_hashmap_.insert(LabelLabelPair(index_,key_));
                key_+=1;
            }
        };
        
        sparse_vector(
            VectorOfLabels elements_indices, VectorOfDoubles nonzero_elements,  LabelLabelMap elements_hashmap
        )
        :
        vector(),
        elements_indices_(elements_indices),
        nonzero_elements_(nonzero_elements),
        elements_hashmap_(elements_hashmap),
        size_(0)
        {
            this->update_size_();
        };
        
        sparse_vector(const sparse_vector& v)
        :
        vector(),
        elements_indices_(v.elements_indices_),
        nonzero_elements_(v.nonzero_elements_),
        elements_hashmap_(v.elements_hashmap_)
        {};
        
        sparse_vector():vector(),size_(0){};

        const Double& operator [] (const Label& index) {
            if(elements_hashmap_.find(index) == elements_hashmap_.end()){
                return this->zero();
            }
            else{
                auto key_ = elements_hashmap_.find(index)->second;
                return nonzero_elements_[key_];
            }
        };
        const Double& operator [] (const Label& index) const {
           if(elements_hashmap_.find(index) == elements_hashmap_.end()){
                return this->zero();
            }
            else{
                auto key_ = elements_hashmap_.find(index)->second;
                return nonzero_elements_[key_];
            }
        };

        Double& operator () (const Label& index){
            if(elements_hashmap_.find(index) == elements_hashmap_.end()){
                this->insert_(0.0,index);
            }
            auto key_ = elements_hashmap_.find(index)->second;
            return nonzero_elements_[key_];
        }

        friend std::ostream& operator << (std::ostream& os, const sparse_vector& v){
            Label v_size_ = v.nonzero_elements_.size();
            Label iter_ = 0 ;
            for(const auto& value_:v.nonzero_elements_){
                os<<"element index "<<v.elements_indices_[iter_]<<", element value is "<<value_;
                iter_+=1;
                if(iter_<v_size_){
                    os<<"\n";
                }
            }
            os<<"\n";
            return os;
        } 

        void insert(Double value, Label index){
            this->insert_(value,index);
        }

        Double operator & (const sparse_vector& v)  {
            Double inner_product_ = 0.0 ;
            Label iter_ = 0 ;
            for(const auto& value_ : nonzero_elements_){
                inner_product_+=(value_*v[elements_indices_[iter_]]);
                iter_+=1;
            }
            return inner_product_;
        };

        Double operator & (const sparse_vector& v) const{
            Double inner_product_ = 0.0 ;
            Label iter_ = 0 ;
            for(const auto& value_ : nonzero_elements_){
                inner_product_+=(value_*v[elements_indices_[iter_]]);
                iter_+=1;
            }
            return inner_product_;
        }

        dense_vector to_dense(){
            auto size = this->size();
            auto values_ = VectorOfDoubles(size,0.0);
            Label i = 0;
            for(const auto& index:elements_indices_){
                values_[index]=nonzero_elements_[i];
                i+=1;
            }
            return dense_vector(values_);
        };

        dense_vector to_dense() const {
            auto size = this->size();
            auto values_ = VectorOfDoubles(size,0.0);
            Label i = 0;
            for(const auto& index:elements_indices_){
                values_[index]=nonzero_elements_[i];
                i+=1;
            }
            return dense_vector(values_);
        };

        Double operator & (const dense_vector& v){
            Double inner_product_ = 0.0 ;
            Label iter_ = 0 ;
            for(const auto& value_ : nonzero_elements_){
                inner_product_+=(value_*v[elements_indices_[iter_]]);
                iter_+=1;
            }
            return inner_product_;
        }

        Double operator & (const dense_vector& v) const {
            Double inner_product_ = 0.0 ;
            Label iter_ = 0 ;
            for(const auto& value_ : nonzero_elements_){
                inner_product_+=(value_*v[elements_indices_[iter_]]);
                iter_+=1;
            }
            return inner_product_;
        }

        Double norm() {
            return std::sqrt(this->operator&(*this));
        };

        Double norm() const{
            return std::sqrt(this->operator&(*this));
        };

        Double sum() {
            Double sum_ = 0 ;
            for(const auto& val:nonzero_elements_){
                sum_+=val;
            }
            return sum_;
        };

        Double sum() const{
            Double sum_ = 0 ;
            for(const auto& val:nonzero_elements_){
                sum_+=val;
            }
            return sum_;
        };

        Double sum_absolutes()  {
            Double sum_ = 0 ;
            for(const auto& val:nonzero_elements_){
                sum_+=std::abs(val);
            }
            return sum_;
        };

        Double sum_absolutes() const{
            Double sum_ = 0 ;
            for(const auto& val:nonzero_elements_){
                sum_+=std::abs(val);
            }
            return sum_;
        };

        sparse_vector operator*(const Double& lambda) {
            VectorOfDoubles scaled_values_(nonzero_elements_);
            for(auto& value_:scaled_values_){
                value_*=lambda;
            };
            return sparse_vector(elements_indices_,scaled_values_,elements_hashmap_);
        };

        sparse_vector operator*(const Double& lambda) const {
            VectorOfDoubles scaled_values_(nonzero_elements_);
            for(auto& value_:scaled_values_){
                value_*=lambda;
            };
            return sparse_vector(elements_indices_,scaled_values_,elements_hashmap_);
        };

        sparse_vector operator/(const Double& lambda)  {
            if(std::abs(lambda) < 1e-14){
                std::cout<<"warning division by almost zero \n";
                Double stable_lambda_ = 1e-14;
                return this->operator*(1/stable_lambda_);
            }
            else{
                return this->operator*(1/lambda);
            }
        };

        void operator += (const sparse_vector& add_v){
            const auto& add_values = add_v.nonzero_elements_;
            const auto& add_indices = add_v.elements_indices_;
            for(const auto& index:add_indices){
                if(elements_hashmap_.find(index) == elements_hashmap_.end()){
                    this->insert_(add_v[index],index);
                }
                else{
                    this->operator()(index)=this->operator()(index)+add_v[index];
                }
            }
        };

        void operator -= (const sparse_vector& add_v){
            const auto& add_values = add_v.nonzero_elements_;
            const auto& add_indices = add_v.elements_indices_;
            for(const auto& index:add_indices){
                if(elements_hashmap_.find(index) == elements_hashmap_.end()){
                    this->insert_(-add_v[index],index);
                }
                else{
                    this->operator()(index)=this->operator()(index)-add_v[index];
                }
            }
        };

        void operator *= (const Double& lambda){
            for(auto& val:nonzero_elements_){
                val*=lambda;
            }
        };

        void operator /= (const Double& lambda){
            for(auto& val:nonzero_elements_){
                val/=lambda;
            }
        }

        sparse_vector operator + (const sparse_vector& add_v){
            auto new_vector = sparse_vector(this->elements_indices_,this->nonzero_elements_,this->elements_hashmap_);
            new_vector+=add_v;
            return new_vector;
        };

        sparse_vector operator + (const sparse_vector& add_v) const {
            auto new_vector = sparse_vector(this->elements_indices_,this->nonzero_elements_,this->elements_hashmap_);
            new_vector+=add_v;
            return new_vector;
        };

        sparse_vector operator - (const sparse_vector& add_v){
            auto new_vector = sparse_vector(this->elements_indices_,this->nonzero_elements_,this->elements_hashmap_);
            new_vector-=add_v;
            return new_vector;
        };

        sparse_vector operator - (const sparse_vector& add_v) const {
            auto new_vector = sparse_vector(this->elements_indices_,this->nonzero_elements_,this->elements_hashmap_);
            new_vector-=add_v;
            return new_vector;
        };

        Label size(){return size_;};

        Label size() const {return size_;};

        void set_size(Label size) {size_ = size;};

    private:
        VectorOfLabels elements_indices_;
        VectorOfDoubles nonzero_elements_;
        LabelLabelMap elements_hashmap_;
        Label size_ ;
        void update_size_(){
            size_*=0.0;
            for(const auto& index:elements_indices_){
                if (index>size_){
                    size_=index;
                }
            }
        }
        void insert_(Double value, Label index){
            auto pushed_index = nonzero_elements_.size();
            if(elements_hashmap_.find(index) == elements_hashmap_.end()){
                elements_hashmap_.insert(LabelLabelPair(index, pushed_index));
                elements_indices_.push_back(index);
                nonzero_elements_.push_back(value);
                if (index>size_){
                    size_=index;
                }
            }
        }
        
};

    class row_or_col: sparse_vector{
    public:
        row_or_col(
            VectorOfLabels elements_indices, 
            VectorOfDoubles nonzero_elements,
            Label row_index,
            Label size
        )
        :
        raw_vector_(sparse_vector(elements_indices,nonzero_elements,size)),
        raw_index_(row_index),
        size_(size)
        {};
        row_or_col(){};
            
        void set_index(Label index){
            raw_index_ = index;
        }

        Label get_index(){
            return raw_index_;
        }

        const sparse_vector& raw_vector() {return raw_vector_;};
        const sparse_vector& raw_vector() const {return raw_vector_;};
        sparse_vector& raw_vector_ref() {return raw_vector_;};

        void set_elements(VectorOfLabels elements_indices, VectorOfDoubles nonzero_elements)
        {
            raw_vector_=sparse_vector(elements_indices,nonzero_elements);
        }

        void set_row_vector(sparse_vector raw_vector){
            raw_vector_=raw_vector;
        }

        Double operator & (const row_or_col& another_raw){
            return (this->raw_vector_ & another_raw.raw_vector_);
        }

        Double operator & (const sparse_vector& vec){
            return (this->raw_vector_ & vec);
        }

        Double operator & (const dense_vector& vec){
            return (this->raw_vector_ & vec);
        }

        Double operator & (const row_or_col& another_raw) const{
            return (this->raw_vector_ & another_raw.raw_vector_);
        }

        Double operator & (const sparse_vector& vec) const{
            return (this->raw_vector_ & vec);
        }

        Double operator & (const dense_vector& vec) const{
            return (this->raw_vector_ & vec);
        }

        void operator += (const row_or_col& another_row){
            this->raw_vector_+=another_row.raw_vector_;
        }

        void operator -= (const row_or_col& another_row){
            this->raw_vector_-=another_row.raw_vector_;
        }

        void operator *= (const Double& lambda){
            this->raw_vector_*=lambda;
        }

        void operator /= (const Double& lambda){
            this->raw_vector_/=lambda;
        }

        row_or_col operator + (const row_or_col& another_row){
            row_or_col new_row_or_col(*this);
            new_row_or_col+=another_row;
            return new_row_or_col;
        }

        row_or_col operator - (const row_or_col& another_row){
            row_or_col new_row_or_col(*this);
            new_row_or_col-=another_row;
            return new_row_or_col;
        }

        row_or_col operator * (const Double& lambda){
            row_or_col new_row_or_col(*this);
            new_row_or_col*=lambda;
            return new_row_or_col;
        }

        row_or_col operator / (const Double& lambda){
            row_or_col new_row_or_col(*this);
            new_row_or_col/=lambda;
            return new_row_or_col;
        }

        const Double& operator [] (Label index){
            return this->raw_vector_[index];
        }

        const Double& operator [] (Label index) const {
            return this->raw_vector_[index];
        }

        Double& operator () (Label index){
            return this->raw_vector_(index);
        }

        void insert(Double value, Label index){
            raw_vector_.insert(value, index); 
        }

        void set_size(Label size){raw_vector_.set_size(size);};
        
        private:
            Label raw_index_;
            sparse_vector raw_vector_;
            Label size_;

    };

    class sparse_matrix{
        public:
            sparse_matrix(CartesianVector indices, VectorOfDoubles non_zero_values, 
            Label m, Label n):
            m_(m),n_(n),
            indices_(indices),non_zero_values_(non_zero_values)
            {
                rows_.resize(m_);
                Label k = 0;
                for(const auto& index:indices){
                    auto i = index.row();
                    auto j = index.col();
                    auto& row = rows_[i];
                    row.insert(non_zero_values_[k],j);
                    k+=1;
                }
                k=0;
                for(auto& row:rows_){
                    row.set_index(k);
                    row.set_size(n_);
                    k+=1;
                }
            };
            
            VectorOfLabels size(){return VectorOfLabels({m_,n_});};

            VectorOfLabels size() const {return VectorOfLabels({m_,n_});};

            const row_or_col& operator [] (Label i){
                return rows_[i];
            };

            const row_or_col& operator [] (Label i) const {
                return rows_[i];
            };

            row_or_col& operator () (Label i){
                return rows_[i];
            };

            Double& operator () (Label i,Label j){
                return rows_[i](j);
            };

            void operator +=(const sparse_matrix& another_matrix){
                Label i = 0 ; 
                for(const auto& row: another_matrix.rows_){
                    this->rows_[i]+=row;
                    i+=1;
                }
            }

            void operator -=(const sparse_matrix& another_matrix){
                Label i = 0 ; 
                for(const auto& row: another_matrix.rows_){
                    this->rows_[i]-=row;
                    i+=1;
                }
            }

            void operator *=(const Double& lambda){
                for(auto& row: this->rows_){
                    row*=lambda;
                }
            }

            void operator /=(const Double& lambda){
                for(auto& row: this->rows_){
                    row/=lambda;
                }
            }

            dense_vector operator&(const dense_vector& v){
                auto vector_size = this->m_;
                dense_vector product(vector_size);
                Label i = 0;
                for(auto& val:product){
                    val = (this->rows_[i] & v);
                    i+=1;
                }
                return product;
            }

            dense_vector operator& (const dense_vector& v) const{
                auto vector_size = this->m_;
                dense_vector product(vector_size);
                Label i = 0;
                for(auto& val:product){
                    val = (this->rows_[i] & v);
                    i+=1;
                }
                return product;
            }

            sparse_matrix(){};

            sparse_matrix T() const{
                CartesianVector transposed_indices(indices_);
                for(auto& index:transposed_indices){
                    index.T();
                }
                return sparse_matrix(transposed_indices,non_zero_values_,n_,m_);
            };

        private:

            RowSpace rows_;
            CartesianVector indices_;
            VectorOfDoubles non_zero_values_;
            Label n_;
            Label m_;
    };

    class gauss_seidel{
        public:
            gauss_seidel(
                const sparse_matrix& A, const dense_vector& b, 
                int max_iter=1000,double min_residual=0.001
            )
            :
            A_(A),
            b_(b),
            solution_(b.size()),
            residual_(1.0),
            max_num_iter_(max_iter),
            min_residual_(min_residual)
            {};
        
            dense_vector solve(){
                this->solve_();
                return solution_;
            }

        private:
            const sparse_matrix& A_;
            const dense_vector& b_;
            dense_vector solution_;
            double residual_;
            int max_num_iter_;
            double min_residual_;
            void solve_(){
                auto mat_size_ = A_.size();
                const auto& n_rows_ = mat_size_[0];
                solution_.resize_with_value(n_rows_,0.0);
                int iter_ = 0;
                for(iter_ = 0; iter_<max_num_iter_; iter_++){
                    std::cout<<"iteration "<<iter_<<" residual "<<residual_<<"\n";
                    if(residual_<min_residual_){
                        break;
                    }
                    else{
                        for(int i=0; i<n_rows_; i++){
                            Double sigma_ = 0.0 ;                            
                            auto a_ii_ = A_[i][i];
                            auto dot_product = A_[i] & solution_;
                            sigma_+=(dot_product-a_ii_*solution_[i]);
                            solution_(i) = (b_[i]-sigma_)/a_ii_;
                        }
                        auto rhs = A_ & solution_;
                        auto diff_ = b_ - rhs;
                        residual_ = diff_.sum_absolutes();
                    }
                }
                std::cout<<"GS no. iteration is "<<iter_;
                std::cout<<" , residual is "<<residual_<<"\n";
            }
    };

    class conjugate_gradient{
        public:
            conjugate_gradient(
                const sparse_matrix& A, const dense_vector& b, 
                Label max_iter=1000, Double min_residual=0.001
            ):
            A_(A),b_(b),solution_(b.size()),residual_(1.0),max_num_iter_(max_iter),min_residual_(min_residual){};
            
            dense_vector solve(){
                this->solve_();
                return solution_;
            }
        private:
            const sparse_matrix& A_;
            const dense_vector& b_;
            dense_vector solution_;
            Double residual_;
            Label max_num_iter_;
            Double min_residual_;
            
            void solve_(){
                auto mat_size_ = A_.size();
                const auto& n_rows_ = mat_size_[0];
                solution_.resize_with_value(n_rows_,0.0);
                auto rhs_ = A_ & solution_;
                auto diff_ = b_ - rhs_;
                auto p_ = diff_;
                int iter_ = 0;

                for(iter_ = 0; iter_<max_num_iter_; iter_++){
                    std::cout<<"iteration "<<iter_<<" residual "<<residual_<<"\n";
                    if(residual_<min_residual_){
                        break;
                    }
                    else{
                        auto num_ = std::pow(diff_.norm(),2);
                        auto den_ = (A_ & p_) & p_ ;                        
                        auto alpha_ = num_/den_;
                        solution_ += alpha_*p_;
                        diff_ -= alpha_* (A_ & p_);
                        auto beta_ = std::pow(diff_.norm(),2)/num_;
                        p_ = diff_ + beta_*p_;
                        residual_ = diff_.sum_absolutes();
                    }
                }
                std::cout<<"CG no. iterations is "<<iter_;
                std::cout<<" , residual is "<<residual_<<"\n";
            }
    };

    class biconjugate_gradient{
        public:
            biconjugate_gradient(const sparse_matrix& A, const dense_vector& b, int max_iter=1000,double min_residual=0.001):
            A_(A),b_(b),solution_(b.size()),solution_tilde_(b.size()),residual_(1.0),residual_tilde_(1.0),max_num_iter_(max_iter),min_residual_(min_residual){};
            
            dense_vector solve(){
                this->solve_();
                return solution_;
            }
        private:
            const sparse_matrix& A_;
            const dense_vector& b_;
            dense_vector solution_;
            dense_vector solution_tilde_;
            double residual_;
            double residual_tilde_;
            int max_num_iter_;
            double min_residual_;
            void solve_(){
                auto mat_size_ = A_.size();
                const auto& n_rows_ = mat_size_[0];
                solution_.resize_with_value(n_rows_,0.0);
                solution_tilde_.resize_with_value(n_rows_,0.0);
                auto rhs_ = A_ & solution_;
                auto diff_          = b_ - rhs_;
                auto diff_tilde_    = b_ - rhs_;
                auto p_             = diff_;
                auto p_tilde_       = diff_;
                int iter_ = 0 ;
                for(iter_ = 0; iter_<max_num_iter_; iter_++){
                    if(residual_<min_residual_){
                        break;
                    }
                    else{
                        auto num_ = diff_ & diff_tilde_;
                        auto den_ = (A_ & p_ ) & p_tilde_ ;
                        auto alpha_ = num_/den_;
                        solution_ += alpha_*p_;
                        solution_tilde_ += alpha_*p_tilde_;
                        diff_ -= alpha_* (A_ & p_);
                        diff_tilde_ -= alpha_* (A_& p_tilde_);
                        auto beta_ =  (diff_ & diff_tilde_)/num_;
                        p_ = diff_ + beta_*p_;
                        p_tilde_ = diff_ + beta_*p_tilde_;
                        residual_ = diff_.sum_absolutes();
                        residual_tilde_ = diff_tilde_.sum_absolutes();
                    }
                }
                std::cout<<"BCG no. iterations is "<<iter_;
                std::cout<<" , residual is "<<residual_;
                std::cout<<" , adjoint residual is "<<residual_tilde_<<"\n";
            }
    };

    class stabilized_biconjugate_gradient{
        public:
            stabilized_biconjugate_gradient(const sparse_matrix& A, const dense_vector& b, int max_iter=1000,double min_residual=0.001):
            A_(A),b_(b),solution_(b.size()),solution_tilde_(b.size()),residual_(1.0),residual_tilde_(1.0),max_num_iter_(max_iter),min_residual_(min_residual){};
            
            dense_vector solve(){
                this->solve_();
                return solution_;
            }
        private:
            const sparse_matrix& A_;
            const dense_vector& b_;
            dense_vector solution_;
            dense_vector solution_tilde_;
            double residual_;
            double residual_tilde_;
            int max_num_iter_;
            double min_residual_;
            void solve_(){
                auto mat_size_ = A_.size();
                const auto& n_rows_ = mat_size_[0];
                const auto& n_cols_ = mat_size_[1];
                solution_.resize_with_value(n_rows_,0.0);
                solution_tilde_.resize_with_value(n_rows_,0.0);
                auto AT_ = A_.T();
                dense_vector s_(n_rows_,0.0);
                dense_vector s_tilde_(n_rows_,0.0);
                auto r_          = b_ - (A_ & solution_);
                auto r0_         = r_;
                auto r_tilde_    = r_;
                dense_vector p_(n_rows_,0.0);
                auto nu_       = p_;
                double rho_         = 1.0;
                double alpha_       = 1.0;
                double omega_       = 1.0;
                int iter_ = 0 ;
                for(iter_ = 0; iter_<max_num_iter_; iter_++){
                    if(residual_<min_residual_){
                        break;
                    }
                    else{
                        auto rho_prev_ = rho_;
                        rho_ = r0_ & r_;
                        auto beta_ = (rho_/rho_prev_)*(alpha_/omega_);
                        p_ = r_ + beta_*(p_ - omega_* nu_);
                        nu_ = A_&p_;
                        alpha_ = rho_/ (r0_ & nu_ );
                        auto h_ = solution_ + alpha_*p_;
                        residual_ = r_.sum_absolutes();
                        if(residual_<min_residual_){
                            solution_ = h_;
                            break;
                        }
                        else{
                            auto s_ = r_ - alpha_*nu_;
                            auto t_ = A_&s_;
                            omega_ = (t_ & s_) / (t_ & t_);
                            solution_ = h_ + omega_*s_;
                            r_ = s_ - omega_*t_;
                            residual_ = r_.sum_absolutes();
                        }
                    }
                }
                std::cout<<"BiCGStab no. iterations is "<<iter_;
                std::cout<<" , residual is "<<residual_<<"\n";
            };
    };

    class successive_over_relaxation{
        public:
            successive_over_relaxation(const sparse_matrix& A, const dense_vector& b, double omega = 1, int max_iter=1000,double min_residual=0.001):
            A_(A),b_(b),solution_(b.size()),residual_(1.0),max_num_iter_(max_iter),min_residual_(min_residual),omega_(omega){};
            
            dense_vector solve(){
                this->solve_();
                return solution_;
            }
        private:
            double omega_;
            const sparse_matrix& A_;
            const dense_vector& b_;
            dense_vector solution_;
            double residual_;
            int max_num_iter_;
            double min_residual_;
            void solve_(){
                auto mat_size_ = A_.size();
                const auto& n_rows_ = mat_size_[0];
                solution_.resize_with_value(n_rows_,0.0);
                int iter_ = 0;
                
                for(iter_ = 0; iter_<max_num_iter_; iter_++){
                    std::cout<<"iteration "<<iter_<<" residual "<<residual_<<"\n";
                    if(residual_<min_residual_){
                        break;
                    }
                    else{
                        for(int i=0; i<n_rows_; i++){
                            Double sigma_ = 0.0 ;                            
                            auto a_ii_ = A_[i][i];
                            auto dot_product = A_[i] & solution_;
                            sigma_+=(dot_product-a_ii_*solution_[i]);
                            auto predicted_ = (b_[i]-sigma_)/a_ii_;
                            auto sol_ = (b_[i]-sigma_)/a_ii_;
                            solution_(i) = (1.0-omega_)*sol_ + omega_*predicted_;
                        }
                        auto rhs = A_ & solution_;
                        auto diff_ = b_ - rhs;
                        residual_ = diff_.sum_absolutes();
                    }
                }
                
                std::cout<<"Sucessive over relaxation no. iteration is "<<iter_;
                std::cout<<" , residual is "<<residual_;
                std::cout<<", relxation factor is "<<omega_<<"\n";
            }
    };
};
#endif /* LWLS_HPP */