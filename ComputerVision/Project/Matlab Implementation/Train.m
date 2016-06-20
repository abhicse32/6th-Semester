train = zeros(40*5,256*40) ;
group = zeros(40*5,1) ;
person = 1 ;
for i = 1:person_count
    for j = 1:case_count
        row = zeros(1,256*40);
        count = 1 ;
        for p = 1:5
            for q = 1:8
                row(count:(count+255)) = mag_train{i,j}{p,q}(:) ;
                count = count + 256 ;                
            end
        end
        train((i-1)*5+j,:) = row(:) ;    
        if i == person
            group((i-1)*5+j) =  1;
        else
            group((i-1)*5+j) =  -1;
        end    
    end
end
disp('generated dataset') ;
svm = svmtrain(train,group) ;
        