function TestX3P()
% TestX3P: Test the x3p integration
%   Read all demo files and display its contents and file information
%
%  This file is part of the openGPS(R)[TM] software library.
%
%  This program is free software; you can redistribute it and/or modify
%  it under the terms of the GNU Lesser General Public License (LGPL)
%  as published by the Free Software Foundation; either version 3 of
%  the License, or (at your option) any later version.
%  for detail see the files "licence_LGPL-3.0.txt" and
%  "licence_GPL-3.0.txt".
%
%  openGPS is distributed in the hope that it will be useful,
%  but WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%  GNU Lesser General Public License for more details.
%
%  You should have received a copy of the GNU General Public License
%  along with this program.  If not, see <http://www.gnu.org/licenses/>.
%
%  The name "openGPS" and the logo are registered as 
%  European trade mark No. 006178354 for
%  Physikalisch Technische Bundesanstalt (PTB)
%  http://www.ptb.de/
%
%  More information about openGPS can be found at
%  http://www.opengps.eu/

%  Copyright 2008, Dr. Georg Wiora, NanoFocus AG



  % Close windows from last run
  close all hidden;
  
  % List of test sample files and files generated by demo programm
  testfiles={ ...
    'ISO5436-sample1.x3p', ...
    'ISO5436-sample1_bin.x3p',...
    'ISO5436-sample2.x3p',...
    'ISO5436-sample3.x3p',...
    'ISO5436-sample4.x3p',...
    'ISO5436-sample4_bin.x3p',...
    'simple.x3p',...
    'medium.x3p',...
    'performance_double.x3p',...
    'performance_double_bin.x3p',...
    'performance_int16.x3p',...
    'performance_int16_bin.x3p'...
    };
  
  % Test all files
  for i=1:numel(testfiles)
    disp(' ');
    disp('****************************************************');
    disp(['Reading x3p file "',testfiles{i},'"...']);
    [z,x,y,pinfor,meta] = openX3P(testfiles{i});
    
    % Print point info
    pinfor
    
    % Test write
    disp(['Write file back to "','w_',testfiles{i},'"']);
    testfilename = ['w_',testfiles{i}];
    
    % if  axis is incremental the coordinate array has to be 1 dimensional
    if pinfor.isXIncremental
      x1 = x(:,1);
    else
      x1 = x;
    end
    if pinfor.isYIncremental
      y1 = y(:,1);
    else
      y1 = y;
    end
    
    pinfow = writeX3P(testfilename,pinfor.FeatureType,x1,y1,z,meta);
    
    % Print point info
    pinfow

    disp(['Rereading and comparing x3p file "',testfiles{i},'"...']);
    [z1,x1,y1,pinfow1,meta1] = openX3P(testfilename);
    
    % Compare meta information
    if structcompare(meta,meta1)==false
      warning('Warning: Meta information has changed on read write cycle!');
    end
    % Compare point information
    if structcompare(pinfow,pinfow1)==false
      pinfow1
      warning('Warning: Point information structure has changed on read write cycle!');
    end
    % Compare coordinate vectors
    ErrMaxX = max(abs(x1(:)-x(:)));
    ErrMaxY = max(abs(y1(:)-y(:)));
    ErrMaxZ = max(abs(z1(:)-z(:)));
    fprintf(1,'Maximum coordinate differences [x,y,z] = [%g,%g,%g]\n',ErrMaxX,ErrMaxY,ErrMaxZ);
    
    % Get dimensions and extend to 3
    dims = [size(z),1,1];
    dims = dims(1:3);
    fprintf(1,'Data dimensions: double(%d,%d,%d)\n',dims);
    % enough data for a plot?
    if dims(1) > 1
      for layer=1:dims(3)
        % Check for data organization
        % Plot data for all layers
        figure('name',testfiles{i});

        % check for list type data
        if pinfor.IsList
          % List type data are independet points without topologic 
          % neighbourship
          plot3(x,y,z,'+');
        else
          % Matrix type data can be ploted as surface or profile
          % Is a surface profile?
          if strcmp(pinfor.FeatureType,'SUR')
            % This is a matrix plot it layer by layer
            hold all;
            surface(x(:,:,layer),y(:,:,layer),z(:,:,layer));
            hold off;
          elseif strcmp(pinfor.FeatureType,'PRF')
            % This is a a line profile
            plot(x,z);
          else
            warning('Unknown profile type, neither "PRF" nor "SUR"!')
          end
        end
      end
    else
        % Just print the data
        x
        y
        z
    end
    % Display meta information for this files
    meta
  end
end

% Helper function to compare two structures
function equal = structcompare(s1,s2)
  % Sort structures
  s1s = orderfields(s1);
  s2s = orderfields(s2);
  
  % Get fieldnames
  f1n = fieldnames(s1s);
  f2n = fieldnames(s2s);
  
  % Compare number of fields
  if numel(f1n) ~= numel(f2n)
    equal=false;
    return;
  end

  % compare
  for i=1:numel(f1n)
    % compare fieldnames
    if ~strcmp(f1n(i),f2n(i))
      equal=false;
      return;
    end
    % compare contents
    if (isnumeric(s1.(f1n{i})) && isnumeric(s2.(f2n{i}))) || ...
       (islogical(s1.(f1n{i})) && islogical(s2.(f2n{i})))
      % Compare numerically
      if s1.(f1n{i}) ~= s2.(f2n{i})
        equal=false;
        return;
      end
    else
      % Compare strings
      if ~strcmp(s1.(f1n{i}),s2.(f2n{i}))
        equal=false;
        return;
      end
    end
  end
  
  % All equal
  equal=true;
end
