clear ;

num_scales = 5 ;
num_orient = 8 ;
kernel_row = 39 ;
kernel_col = 39 ;

person_count = 22 ;
case_count = 7 ;

cd('feret')
people = dir ;
people = people(3:person_count+2) ;
paths = [] ;
for i = 1:22
    cd(people(i).name)
    y = dir;
    files = y(3:9);
    cd ..
    paths = [paths;files'];
end
cd ..

main;
lda;
knn;
