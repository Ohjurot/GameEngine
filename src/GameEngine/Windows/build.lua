mox_project("Windows", "gameengine_windows")
mox_cpp("C++20")
mox_staticlib()

dependson { 
    "imgui", 
}
