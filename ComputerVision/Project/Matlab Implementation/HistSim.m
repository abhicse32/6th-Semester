function [ sim ] = HistSim( IM1,IM2 )
%HISTSIM Summary of this function goes here
%   Detailed explanation goes here
    sim = zeros(5,8) ;
    for i = 1:5
        for j = 1:8
            sim(i,j) = measure(IM1{i,j},IM2{i,j});
        end
    end    
end

function [val] = measure(H1,H2)
    val = 0 ;
    for i = 1:256
        val = val + abs(H1(i)-H2(i)) ;
    end    
end