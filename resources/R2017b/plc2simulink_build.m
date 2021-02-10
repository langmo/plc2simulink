% Build script for PLCSimAdvancedSFunction
includePath = fullfile(cd(), 'src', 'include');
cppFiles = dir(fullfile('src', 'lib', '*.cpp'));
cppFiles = arrayfun(@(x)fullfile(x.folder, x.name), cppFiles, 'UniformOutput', false);
mex(['-I',includePath],...
    '-output', 'plc2simulink',...
    fullfile('src', 'lib', '*.cpp'));



