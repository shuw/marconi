function filename = testNameToFilename(testName)
% filename = testNameToFilename(testName)
%   Converts the given textual test name (which may contain spaces, commas, etc.) 
%   to a more operating-system friendly filename (illegals replaced with underscores).

    testName = regexprep(testName, '\s*[,.:;]+\s*', '_');
    filename = regexprep(testName, '\s', '_');
end
