## GitHub Embassy
Not a big fan of Hack Club Embassy, so I made my own.
## Hosted Demo
I hosted it at https://embassy.faked.my.id/ \
I wanted to host it at https://no.joke.im.not.lying.this.is.real.yeah.so.embassy.has.actually.forged.or.faked.my.id/ but then SSL wouldnt work so I used the shorter domain
## Inspirations
- I took the Sign In page from https://github.com/M-Aayan/github-signin-page and did small changes / mostly removed most of the fields.
- I took the entire website from embassy.hackclub.com just not the signin.
## Compiling
I used VCPKG to install the packages
Install the following packages in VCPKG, it will automatically be installed when building on linux but on windows has to be installed manually:
- nlohmann/json
- httplib
I also included the linux files for compiling with CMake \
Also get https://github.com/microsoft/vcpkg before compiling, thats what manages VCPKG
## Running the server
You can copy the [assets.zip](https://github.com/PhasedGapple/Github-Embassy/blob/master/assets.zip) and extract it in the folder with the binary to run the server, then in config.json make sure to set your personal fine grained token from github.
## Usage of AI
I used AI for Research, Documenting, writing small and unnoticable parts of the javascript of the hotend, troubleshooting, improving the backend, debugging and to recommend me ways on how to do my code.
