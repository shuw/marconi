global PLOT_PATH;
global PLOT_EXTENSION;
global SAMPLE_RATE;

PLOT_PATH = '.\plots\';
PLOT_EXTENSION = '.bmp';
SAMPLE_RATE = 22050;

if ~exist(PLOT_PATH)
    mkdir(PLOT_PATH);
end
