train = zeros(person_count*case_count,256*num_scales*num_orient) ;
group = zeros(person_count*case_count,1) ;

for i = 1:person_count
    for j = 1:case_count
        row = [] ;%zeros(1,256*num_scales*num_orient);
        %count = 1 ;
        for p = 1:num_scales
            for q = 1:num_orient
                row = [row,mag_train{i,j}{p,q}(:)'] ;
                %row(count:(count+255)) = mag_train{i,j}{p,q}(:) ;
                %count = count + 256 ;                
            end
        end
        train((j-1)*person_count+i,:) = row(:) ;    
        group((j-1)*person_count+i) =  i;    
    end
end

sep = 88 ;
total = person_count*case_count ;
train_set = train(1:sep,:) ;
save('train_set.mat','train_set') ;
train_id = group(1:sep) ;
save('train_id.mat','train_id') ;

test_set = train((sep+1):total,:) ;
save('test_set.mat','test_set') ;
test_id = group((sep+1):total) ;
save('test_id.mat','test_id') ;

model = perform_lda_PhD(train_set',train_id,person_count-1);
test_features = linear_subspace_projection_PhD(test_set', model, 1);