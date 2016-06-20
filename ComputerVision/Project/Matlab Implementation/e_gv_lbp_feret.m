%%function to return histogram
function [Maghist, Phasehist, mag_lbp_codes, phase_lbp_codes] = e_gv_lbp_feret(image, filterBank)
%function [gaborMagFaces] = e_gv_lbp(image, filterBank)
%% Calculating Gabor gaborFaces

    u = size(filterBank,1) ;
    v = size(filterBank,2) ;

    gaborMagFaces = cell(u,v) ;
    gaborPhaseFaces = cell(u,v) ;
    %gaborFaces = cell(u,v) ;
    
    for i = 1:u
        for j = 1:v
            ab = conv2(double(image),filterBank{i,j},'same') ;
            %gaborFaces{i,j} = ab ;
            gaborMagFaces{i,j} = abs(ab);
            gaborPhaseFaces{i,j} = angle(ab);
        end    
    end
    
    m = size(gaborMagFaces{1,1},1) ;
    n = size(gaborMagFaces{1,1},2) ;
    
%% Calculating the E GV LBP values
    
%     zero = zeros(m+2,n+2);
%     tempgaborFaces = cell(u+2,v+2);
%     
%     for i = 1:(u+2)
%         for j = 1:(v+2)
%             tempgaborFaces{i,j} = zero;
%         end
%     end    
%     
%     for i = 1:u
%         for j = 1:v
%             tempgaborFaces{i+1,j+1}(2:(m+1),2:(n+1)) = filterBank{i,j};
%         end
%     end 
%     
    mag_lbp_codes = cell(u,v) ;
    phase_lbp_codes = cell(u,v) ;
    Maghist = cell(u,v);
    Phasehist = cell(u,v);
    
    for a = 1:u
        for b = 1:v
            magtemp = zeros(m,n) ;
            phasetemp = zeros(m,n) ;
            localMaghist = zeros(1,256);
            localPhasehist = zeros(1,256);
            for c= 1:m
                for d = 1:n
                    magval = lbp(a,b,c,d,u,v,m,n,gaborMagFaces) ;
                    magtemp(c,d) = magval ;
                    localMaghist(magval+1) = localMaghist(magval+1) + 1 ;
                    phaseval = lbp(a,b,c,d,u,v,m,n,gaborPhaseFaces) ;
                    phasetemp(c,d) = phaseval ;
                    localPhasehist(phaseval+1) = localPhasehist(phaseval+1) + 1 ;
                end
            end
            mag_lbp_codes{a,b} = magtemp ;
            Maghist{a,b} = localMaghist/sum(localMaghist) ;
            phase_lbp_codes{a,b} = phasetemp ;
            Phasehist{a,b} = localPhasehist/sum(localMaghist) ;
        end
    end
    
end

%%function to calculate lbp score
function val = lbp(a,b,c,d,u,v,m,n,gaborFaces)
    
    val = 0 ;
    ic = gaborFaces{a,b}(c,d) ;
    
    % -ve orientation 
    if(b - 1 <= 0)
        ip = 0 ;
    else
        ip = gaborFaces{a,b-1}(c,d) ;
    end
    if(ip >= ic)
        val = val + 1 ;
    end    
    
    % -ve row 
    if(c - 1 <= 0)
        ip = 0 ;
    else
        ip = gaborFaces{a,b}(c-1,d) ;
    end
    if(ip >= ic)
        val = val + 2 ;
    end
    
    % +ve scale 
    if(a + 1 > u)
        ip = 0 ;
    else
        ip = gaborFaces{a+1,b}(c,d) ;
    end
    if(ip >= ic)
        val = val + 4 ;
    end
    
    % +ve col 
    if(d + 1 > n)
        ip = 0 ;
    else
        ip = gaborFaces{a,b}(c,d+1) ;
    end
    if(ip >= ic)
        val = val + 8 ;
    end
    
    % +ve orientation 
    if(b + 1 > v)
        ip = 0 ;
    else
        ip = gaborFaces{a,b+1}(c,d) ;
    end
    if(ip >= ic)
        val = val + 16 ;
    end
    
    % +ve row
    if(c + 1 > m)
        ip = 0 ;
    else
        ip = gaborFaces{a,b}(c+1,d) ;
    end
    if(ip >= ic)
        val = val + 32 ;
    end
    
    % -ve scale 
    if(a - 1 <= 0)
        ip = 0 ;
    else
        ip = gaborFaces{a-1,b}(c,d) ;
    end
    if(ip >= ic)
        val = val + 64 ;
    end
    
    % -ve col 
    if(d - 1 <= 0)
        ip = 0 ;
    else
        ip = gaborFaces{a,b}(c,d-1) ;
    end
    if(ip >= ic)
        val = val + 128 ;
    end
end