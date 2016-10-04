function [output] = readData(s)

% Read value returned via Serial communication 
output = fscanf(s);

end
