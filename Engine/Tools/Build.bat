@ECHO OFF

echo Building CustomImageCreateAll
g++ -g Engine/Tools/CustomImageCreateAll.cpp -IEngine/Vendor/stb -o Engine/Tools/Bin/CustomImageCreateAll
echo Done Building CustomImageCreateAll

echo Building CustomImageCreateIndividual
g++ -g Engine/Tools/CustomImageCreateIndividual.cpp -IEngine/Vendor/stb -o Engine/Tools/Bin/CustomImageCreateIndividual
echo Done Building CustomImageCreateIndividual
