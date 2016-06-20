num_scales = 5 ;
num_orient = 8 ;
kernel_row = 39 ;
kernel_col = 39 ;

person_count = 22 ;
case_count = 7 ;

load('train_set.mat');
load('train_id.mat');

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

filters = gaborFilterBank(num_scales, num_orient, kernel_row, kernel_col) ;
model = perform_lda_PhD(train_set',train_id,person_count-1);

file = 'test/00770_960530_fa_a.pgm' ;
A = imread(file);%imread(strcat('feret/',people(16).name,'/',paths(16,6).name)) ;%imread('orl_faces/s13/8.pgm') ;
A = rgb2gray(A) ;
A = imresize(A,0.5) ;
[Maghist, PhaseHist, mag_lbp, phase_lbp] = e_gv_lbp_feret(A,filters);

row = [] ;%zeros(1,256*num_scales*num_orient);
for p = 1:num_scales
    for q = 1:num_orient
        row = [row,Maghist{p,q}(:)'] ;
        %row(count:(count+255)) = mag_train{i,j}{p,q}(:) ;
        %count = count + 256 ;                
    end
end

test_features = linear_subspace_projection_PhD(row', model, 1);
results = nn_classification_PhD(model.train, train_id, test_features, 2, size(test_features,1), 'mahcos');

[min_arr,I] = sort(results.match_dist) ;
count = zeros(1,person_count) ;

for i = 1:10
    count(train_id(I(i))) = count(train_id(I(i))) + 1;
end   

[max,J] = sort(count(:)) ;

if max(person_count) == 1
    idx = train_id(I(1)) ;
else
    idx = J(person_count) ;
end

subplot(2,10,1);
imshow(file) ;
title(people(idx).name, 'FontSize', 7) ;
for i = 1:10
    subplot(2,10,i+10);
    imgNo = floor(I(i)/person_count + 1);
    str = strcat('feret/',people(train_id(I(i))).name,'/',paths(train_id(I(i)),imgNo).name) ;
    imshow(str) ;
    title(paths(train_id(I(i)),imgNo).name, 'FontSize', 7) ;
end 

% min = results.match_dist(1) ;
% idx = train_id(1) ;
% for i = 2:size(results.match_dist,2)
%     val = results.match_dist(i) ;
%     if val < min
%         min = val ;
%         idx = train_id(i) ;
%     end    
% end    

disp(idx) ;
disp(people(idx).name) ;