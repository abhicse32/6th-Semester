num_scales = 5 ;
num_orient = 8 ;
kernel_row = 39 ;
kernel_col = 39 ;

person_count = 40 ;
case_count = 10 ;

load('orl_faces/train_set.mat');
load('orl_faces/train_id.mat');

filters = gaborFilterBank(num_scales, num_orient, kernel_row, kernel_col) ;
model = perform_lda_PhD(train_set',train_id,person_count-1);

file = 'orl_faces/s4/8.pgm';
A = imread(file) ;%imread('orl_faces/s13/8.pgm') ;
%A = rgb2gray(A) ;
A = imresize(A,0.5) ;
[Maghist, PhaseHist, mag_lbp, phase_lbp] = e_gv_lbp(A,filters);

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
title(idx) ;
for i = 1:10
    subplot(2,10,i+10);
    imgNo = floor(I(i)/person_count + 1);
    imshow(sprintf('orl_faces/s%d/%d.pgm',train_id(I(i)),imgNo)) ;
    title(sprintf('s%d/%d.pgm',train_id(I(i)),imgNo), 'FontSize', 7) ;
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