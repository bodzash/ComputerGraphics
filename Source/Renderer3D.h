#pragma once

/*
    These either contain:
    vertex layouts
    views
    mvp related shit

*/

class RendererData
{

};

class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void BeginRender();
    static void EndRender();
    static void Flush();
};