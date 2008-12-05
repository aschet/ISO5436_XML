function TestX3P()
% TestX3P: Test the x3p integration

  % [z,y,x] = openX3P('ISO5436-sample1_bin.x3p');
  [z,y,x,meta] = openX3P('performance_int16_bin.x3p');
  plot(x,z);
  meta
end
