function output = executeModem(pipeline, input, inType, outType, target)
% output = executeModem(pipeline, input, inType, outType, target)
%   Runs the simulation build of the modem pipeline using the specified input
%   and returning the produced output. The input and output data types are 
%   specified with inType and outType respectively. The specific test pipeline 
%   to execute is specified. Valid targets are 'sim' and 'hwsim'.

    fOut = fopen('input.dat', 'wb');
    fwrite(fOut, input, inType); 
    fclose(fOut);

    fOut = fopen('test_options.dat', 'wb');
    fwrite(fOut, padCString(pipeline, 32), 'char');
    fclose(fOut);

    switch target
        case 'sim'
            file = [pwd '/../firmware/Marconi/Bin/Sim/MarconiSim.exe'];
            [s, w] = system(file);
            %disp(w)
            if s ~= 0
                error('%s\n returned with error', file);
            end
        case 'hwsim'
            disp('Run TI simulator and press any key when complete...');
            pause;
        otherwise
            error(sprintf('Invalid target: %s', target));
    end

    fIn = fopen('output.dat', 'rb');
    output = fread(fIn, outType)';
    fclose(fOut);
end


