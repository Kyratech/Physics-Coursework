
--
-- see https://premake.github.io/ for explanations of these commands.
--
solution ('Physics_CW') --Top Level name of the project
   configurations { 'Release' }
      language 'C++' -- The programming language
      project("Physics_App") -- The specific name
        kind 'ConsoleApp' -- the target application
        targetdir('./') -- where to put the executable
        links{'glew32', 'glfw3', 'opengl32', 'libBulletDynamics', 'libBulletCollision', 'libLinearMath'} -- the libraries to link
        files {"*.cpp"} -- collects all the cpp files in the directory, there must only be one main function.
        buildoptions{'-Wno-write-strings'} -- build option to suppress a very common warning  about strings
