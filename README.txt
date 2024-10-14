`#include <GL/glut.h>` 的 include 方式參照官網: https://www.opengl.org/archives/resources/code/samples/glut_examples/examples/examples.html

# OpenGL 環境
- 使用 [freeglut](https://freeglut.sourceforge.net/), 用 https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-freeglut 載
    - link + 執行需要: C:\Windows\System32\opengl32.dll 和 C:\msys64\ucrt64\lib\libfreeglut.dll.a 引導至 C:\msys64\ucrt64\bin\libfreeglut.dll
    - 或是用目前的 `GLUT_env` folder 也行

## 如何在沒有 `GLUT_env` folder 下編譯 
0. 總之先下 make，沒編譯出 `.exe` 再來看~
    - MinGW 使用者，或許能試試下 `mingw32-make`
1. 開 makefile，確認 "GLUT parameters" 區域
    - USING_GLUT: GLUT 的 `.lib` 或 `lib .dll.a` 的名字
    - GLUT_INCLUDE_ARGU: 放 `glut.h` 的資料夾路徑
    - GLUT_LIB_ARGU: 放 `.lib` 或 `lib .dll.a` 的資料夾路徑
2. 確認 `glut.h` 有沒有放在 `GL` 資料夾下面
    - 有: 請 `#include <GL/glut.h>`
    - 沒: 請 `#include <glut.h>`
3. 確認 GLUT 的 .dll 檔在 PATH ，或在 `.exe` 同一個資料夾下，能被 `.exe` 存取到
4. 確認 GLUT 的 .dll 檔是 64 bits 的，32 bits 版無法 link
5. 還不行？ 我也不知道，請聯繫我！


# 參考資料
- 依我的理解簡化(刻/抄)了 Godot node 系統: https://github.com/godotengine/godot/tree/master
    - nodes 做主要 object, memory management
        - 簡化: Godot Node2D 會經過 CanvaItem Node, 多了 layer 控制功能，我們改採用 Node3D 設計概念: https://github.com/godotengine/godot/blob/master/scene/3d/node_3d.h  
        - 簡化: Godot 以 RenderServer 內容物(World3D) draw, 以達到 implement independent, 我們直接畫出 child nodes 就好
            - 想法、未實作: multi-viewport, 可實作 SubViewport Node, 紀錄主 Viewport, draw 時以主 Viewport child nodes transform 乘上 SubViewport 自己 view transform, 再 render 一次即可
                - 記得 render 要使用原本的 dfs_stack, 主 Viewport 才不會重複 render 到 SubViewport child
        - 簡化: Godot 用神奇 ClassDB 實現 signal(command pattern) in Object Class for C++, C#, GDscript 互通，我們用 delegate.h 實作即可(放在 ./templates 裡)
        - 簡化: Godot 原本會用 Control Node 處理全部 UI ，我改用 Node2D-derived Node 就好~

    - resources 存 data (可以 multiple node 共用 1 resource)
        - 簡化: Godot 的 Line 以 Line2D Node implement, 把底層 Mesh 隱藏起來, 使用上比較方便 + width 能 scaling, 我們改直接用 LineMesh (width 不能 scaling...) 
        - 簡化: Godot 的 ArrayMesh 能用 Vertices Array 外東西去畫, 我們只用 Vertices Array  
        - 簡化: Godot Texture 不只 ImageTexture 還有 MeshTexture, NoiseTexture...，我們只能放 Image, 所以沒有 derived classes
            - 簡化: Godot Image loading 有專門 singleton, 能在不同地方使用 Image，不需要，不實做 Image singleton 了

    - servers 使用 nodes 與 resources，更新 node tree 或 Output  
        - 簡化: Godot 的 resources 真的只存 data, function implement 都在 servers, 這樣能同時支援 OpenGL, Vulkan 等，我們改在 resources 直接 implement，server 不用寫每個 resource 對應接口~
        - 改: Godot 處理 input 功能包在 DisplayServer 裡了，我不太喜歡，自創 InputServer

    - templates 放常用的 data type
    - config 放設定檔
    - thirdparty 放不是我寫的 code: 如 file praser
        - 老師提供的 bmp_io 是用 library(Visual Studio 的)匯入 + 小修正，好像也沒 100% detailed, 所以選擇用完整 image file parse library: stb (一樣回傳 RGB pixel array)
        
- 畫 .bmp: ChatGPT + stackoverflow 互相修正結果
    - https://stackoverflow.com/questions/24262264/drawing-a-2d-texture-in-opengl
    - https://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture

- 開啟 OpenGL 預設 Anti-Aliasing: https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
    - 我只下 glutInitDisplayMode 的 GLUT_MULTISAMPLE 就有用了，反正預設的東西，有就好~
- .ttf 運作原理: https://www.youtube.com/watch?v=caLqFG6w4Mk&ab_channel=Myvar
- if v.s. empty function call: https://stackoverflow.com/questions/10797398/which-is-faster-empty-function-call-or-if-statements
    - function call 比較傷，目前 RenderServer 還是用 if 去看是否 Node 需要 draw

# RD (Reserch & Development)
[V] create windows
    [V] naming

[V] node tree (for position + redraw):
    [V] node: base memory management
    [V] node_2d: position(relative to parent) & transforms (good as pivots)

[] draw primitive(lines, triangles, dots)
    [] storing data structure
        [] basic
            [] vector
            [] stack
            [] singleton check
        [] mesh (store vertex)
            [] lines
            [] triangles
            [] dots
    [] draw command
        [] dfs, calling `draw` from mesh_2d
        [] mesh_2d `draw`
            [X] local > global transforms
                - mesh 存 "原始大小、座標"
                - mesh_instance_2d 存 "transform" by mesh_instance_2d 到 root Node2D 的 transform
                - mesh 以 原始座標 * transform(由 RenderServer 傳入) 得到 global 位置、draw 出來
            [] scaling included
        [] quene_redraw variable + set it in `idle` 
        [] (vertex = 1: dot, vertex = 2: line, vertex = 3: triangles, vertex > 3: multi triangles)
    [] random
[] clear primitives

[] parse .bmp image
    [] data structure
[] draw images
    [] property change
[] clear image

[] parse .ttf fonts
    [] data structure
[] draw fonts
    [] property change
[] make button
    [] property change
    [] interact
    [] animation


# UI/UX
[] color choose
[] font choose
[] UI size
[] UI position
