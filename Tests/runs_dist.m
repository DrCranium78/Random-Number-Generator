%
%  runs_dist.m
% 
%  Created: 30. May. 2022
%   Author: Frank Bjørnø¸
% 
%  Purpose: To read and present data from a file prepared by running the program runsdistr.c
%           The objective is to demonstrate that runs are normally distributed.
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



fmt = '%u';
DATA = [];
sizeDATA = [1, inf];

%  --------------------------    Read Data    --------------------------

	%  'data_file.dat' should be in the same directory as this MatLab file
	%  Both runs_obs.c and runs_exp.c overwrites and stores data in 'datafile.dat' so
	%  this script generates a histogram based on which of these files was run last.
fileID = fopen('datafile.dat', 'r');

if (fileID == -1)
    disp('Error: Could not open data file.')
else
    while ~feof(fileID)
        DATA = fscanf(fileID, fmt, sizeDATA);        
    end
    
    closeresult = fclose(fileID);
    if closeresult ~= 0
        disp('Error: Could not close file.')
    end
end

	% clean up
clear closeresult fileID fmt sizeDATA


%  --------     prepare data for the overlaying normal curve    --------

x  = 0:0.1:16;						%  prepare data points from 0 to 16 on the x axis
y  = normpdf(x, mean(DATA), std(DATA));			%  calculate corresponding data points on the y axis
SI = size(DATA);

%  ----------------------     plot the graphs    -----------------------

	%  plot the histogram of the runs data
f1 = figure(1);
set(gcf, 'position', [400 500 800 380]);
H = histogram(DATA);
H.FaceColor = [.8 .8 1];
H.EdgeColor = [.8 .8 1];
H;

hold on

	%  plot the overlaying normal curve
y = y .* SI(2) * H.BinWidth;				%  scale the y coordinates to fit the histogram
plot(x, y, 'red')
