filters = gaborFilterBank(num_scales, num_orient, kernel_row, kernel_col) ;

mag_train = cell(person_count,case_count) ;
ph_train = cell(person_count,case_count) ;

for i = 1:person_count
    for j = 1:case_count
        str = strcat('feret/',people(i).name,'/',paths(i,j).name);%sprintf('orl_faces/s%d/%d.pgm',i,j) ;
        A = imread(str) ;
        A = imresize(A,0.5) ;
        A = rgb2gray(A) ;
        [Maghist, PhaseHist, mag_lbp, phase_lbp] = e_gv_lbp(A,filters);
        mag_train{i,j} = Maghist ;
        ph_train{i,j} = PhaseHist ;
        disp([i,j]);
    end
end    