function InstallDependencies {
    choco install opencv --version 4.5.4
    choco install choco install boost-msvc-14.1 -y
    choco install cmake -y
}

function BuildProject {
    cd $PSScriptRoot\..\
    mkdir buildWindows
    cd buildWindows
    cmake ..
    cmake --build .
}

$CHOCO_VERSION = choco -v

if ($CHOCO_VERSION) {
    echo "Chocolatey version: " $CHOCO_VERSION

    InstallDependencies
    BuildProject
}



