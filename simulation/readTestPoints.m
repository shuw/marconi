function tp = readTestPoints(filename, testPoints)

    tp = struct;
    for i = 1 : length(testPoints)
        tp = setfield(tp, testPoints{i}.name, []);
    end


    fIn = fopen(filename, 'rb');
    while ~feof(fIn)
        for i = 1 : length(testPoints)
            v = fread(fIn, testPoints{i}.length, testPoints{i}.type)';
            v = [getfield(tp, testPoints{i}.name); v];
            tp = setfield(tp, testPoints{i}.name, v);
        end
    end
    fclose(fIn);

end
