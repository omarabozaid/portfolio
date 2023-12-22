#include <iostream>
#include <string.h>
#include <sndfile.h>
#include <vector>

class SoundAnalyzer {
public:
    SoundAnalyzer(std::string FileName) : FileName_(FileName) {
        OpenSoundFile();
        CloseSoundFile();
    }
    ~SoundAnalyzer() {}

private:
    std::string FileName_;
    SF_INFO SFInfo_;
    SNDFILE* SoundFile_;
    std::vector<double> InputSignal_;

    void OpenSoundFile() {
        SoundFile_ = sf_open(&FileName_[0], SFM_READ, &SFInfo_);
        InputSignal_.resize(SFInfo_.frames);
        sf_read_double(SoundFile_, InputSignal_.data(), int(InputSignal_.size()));
        
        if (!SoundFile_) {
            std::cerr << "Error opening sound file: " ;
            std::cerr<< sf_strerror(SoundFile_) << "\n";
            exit(1);
        }
    }

    void CloseSoundFile() {
        sf_close(SoundFile_);
    }

};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sound_file>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    SoundAnalyzer analyzer(filename);
    return 0;
}
