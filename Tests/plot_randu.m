%
%  plot_randu.m
% 
%  Created: 05. Oct. 2022
%   Author: Frank Bjørnø¸
% 
%  Purpose: Generate 5000 points in 3 dimensional space and plots the. The points are
%           generated using an rng that is known to be flawed and will generate points
%           that form planes i 3d.
% 
%  License:
%  
%           Copyright (C) 2022 Frank BjÃ¸rnÃ¸
% 
%           1. Permission is hereby granted, free of charge, to any person obtaining a copy 
%           of this software and associated documentation files (the "Software"), to deal 
%           in the Software without restriction, including without limitation the rights 
%           to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
%           of the Software, and to permit persons to whom the Software is furnished to do 
%           so, subject to the following conditions:
%         
%           2. The above copyright notice and this permission notice shall be included in all 
%           copies or substantial portions of the Software.
% 
%           3. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
%           INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
%           PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
%           HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
%           CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
%           OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
%    

 V = [];
 sizeV = [3 inf];
    
    %  generate 10000 points in R3
for c = 1:5000      
    V = [V [rndu(); rndu(); rndu()]];     
end

    %  plot the points
f1 = figure(1);
set(gcf, 'position', [400 500 800 480]);
plot3(V(1,:), V(2,:), V(3,:), '.');

    %  set axes limits and enable grid
xlim([0 1]);
ylim([0 1]);
zlim([0 1]);
grid on    


%
%     Implementation of the IBM subroutine RANDU
%
function rndf = rndu()
    
        %  rndf = randu() returns a pseudo random number in 
        %         the closed interval [0, 1]
    
        %  seed retains its value between calls
    persistent seed    
    
        %  initialize seed on the first call
    if isempty(seed)
        seed = 39987;                
    end
    
        %  generate a new seed.
    seed = mod((65539 * seed), 2^31);
    
        %  scale seed to a number between 0 and one
    rndf = seed / 2^31;
end