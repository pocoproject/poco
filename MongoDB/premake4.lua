-- Where is POCO?

poco_dir = "/home/bronx/Development/poco"
poco_lib_dir = "/usr/local/lib"

-- Solution

solution "PocoMongoDB"
  configurations { "Debug", "Release" }
  platforms { "x32", "x64" }

  location ( "build/" .. _ACTION )

  configuration "Debug"
    targetdir "bin/Debug"
    defines { "DEBUG" }
    flags { "Symbols" }

  configuration "Release"
    targetdir "bin/Release"

  configuration { "linux", "Release" }
    libdirs {
              "bin/Release"
            }
  configuration { "linux", "Debug" }
    libdirs {
              "bin/Debug"
            }

project "PocoMongoDB"
  kind "SharedLib"
  language "C++"
  targetname "PocoMongoDB"

  location   ( solution().location )

  configuration "Debug"
    targetsuffix "d"

  configuration { }

  defines { "MongoDB_EXPORTS" }
  
  files { 
            "src/*.cpp"
          , "include/**.h"
        }
  
  libdirs { 
            poco_lib_dir
          }  
          
  includedirs { 
                  poco_dir .. "/Foundation/include"
                , poco_dir .. "/Net/include"
                , "include"
              }  
              
  configuration "Debug"
    links { 
              "PocoNetd" 
            , "PocoFoundationd"
          }
  
  configuration "Release"
    links { 
              "PocoNet" 
            , "PocoFoundation" 
          }

project "MongoDBTestSuite"
  kind "ConsoleApp"
  language "C++"

  configuration "Debug"
    targetsuffix "d"

  configuration { }
  
  files { 
            "testsuite/src/*.cpp"
        }

  configuration "Linux"
  	excludes { "testsuite/src/Win*.*" }

  configuration { }

  includedirs { 
                  poco_dir .. "/Foundation/include"
                , poco_dir .. "/Net/include"
                , poco_dir .. "/CppUnit/include"
                , "include"
              }  
  
  libdirs { 
            poco_lib_dir
          }  
          
  configuration "Debug"
    links { 
              "PocoFoundationd" 
            , "PocoMongoDBd"
            , "CppUnitd"
          }
  
  configuration "Release"
    links { 
              "PocoFoundation" 
            , "PocoMongoDB"
            , "CppUnit"
          }

project "MongoDBSample"
  kind "ConsoleApp"
  language "C++"
  targetname "MongoDBSample"

  configuration "Debug"

    targetsuffix "d"

  configuration { }

  files { 
            "samples/**.cpp"
        }

  includedirs { 
                  poco_dir .. "/Foundation/include"
                , poco_dir .. "/Net/include"
                , "include"
              }  

  libdirs { 
            poco_lib_dir
          }
          
  configuration "Debug"
    links { 
              "PocoFoundationd"
            , "PocoMongoDBd"
          }
  
  configuration "Release"
    links { 
              "PocoFoundation"
            , "PocoMongoDB"
          }
    
