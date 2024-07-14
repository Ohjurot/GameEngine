mox_project("EmptyGame")
mox_cpp("C++20")
mox_console()

-- Project in solution
links {
    "Core", 
    "Windows",
    "imgui"
}

-- System libs
links {
    "d3d12.lib",
    "dxgi.lib",
    "dxguid.lib"
}
