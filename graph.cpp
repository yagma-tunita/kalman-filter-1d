#include "graph.hpp"
#include <windows.h>
#include <vector>
#include <cstdio>
#include <cstring>

namespace
{
    constexpr int kWindowWidth  = 800;
    constexpr int kWindowHeight = 600;
    constexpr int kMargin       = 60;

    const Graph* g_graph = nullptr;

    int MapX(double tip, int datacount)
    {
        int plotW = kWindowWidth - 2 * kMargin;
        return kMargin + (int)(tip / (datacount - 1.0) * plotW);
    }

    int MapY(double y, double yMin, double yMax)
    {
        int plotH = kWindowHeight - 2 * kMargin;
        return kWindowHeight - kMargin
             - (int)((y - yMin) / (yMax - yMin) * plotH);
    }

    COLORREF ColorOf(CurveType type)
    {
        switch (type)
        {
        case CurveType::INPUT: return RGB(255, 0, 0);   // red
        case CurveType::REAL:  return RGB(0, 0, 0);     // black
        case CurveType::PRIOR: return RGB(0, 255, 0);   // green
        }
        return RGB(0, 0, 0);
    }

    void DrawAxes(HDC hdc)
    {
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);

        MoveToEx(hdc, kMargin, kMargin, NULL);
        LineTo(hdc, kMargin, kWindowHeight - kMargin);
        LineTo(hdc, kWindowWidth - kMargin, kWindowHeight - kMargin);

        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }

    void DrawCurve(HDC hdc, const Curve& curve, double yMin, double yMax)
    {
        int n = (int)curve.Size();
        if (n < 2) return;

        HPEN pen = CreatePen(PS_SOLID, 1, ColorOf(curve.Type()));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);

        std::vector<POINT> pts(n);
        for (int i = 0; i < n; i++)
        {
            pts[i].x = MapX((double)i, n);
            pts[i].y = MapY(curve.Data()[i], yMin, yMax);
        }
        Polyline(hdc, pts.data(), n);

        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }

    void DrawXLabels(HDC hdc, int datacount, double xStep)
    {
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));

        const int divisions = 6;
        int step = (datacount - 1) / divisions;
        if (step <= 0) return;

        for (int i = 0; i <= datacount - 1; i += step)
        {
            int sx = MapX((double)i, datacount);
            double xValue = i * xStep;

            char buf[32];
            sprintf(buf, "%.3g", xValue);
            TextOutA(hdc, sx - 15, kWindowHeight - kMargin + 5,
                     buf, (int)strlen(buf));
        }
    }

    void DrawYLabels(HDC hdc, double yMin, double yMax)
    {
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));

        const int divisions = 6;
        double step = (yMax - yMin) / divisions;

        for (int i = 0; i <= divisions; i++)
        {
            double y = yMin + i * step;
            int sy = MapY(y, yMin, yMax);

            char buf[32];
            sprintf(buf, "%.1f", y);
            TextOutA(hdc, kMargin - 45, sy - 8, buf, (int)strlen(buf));
        }
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            DrawAxes(hdc);

            for (const Curve& c : g_graph->Curves())
            {
                DrawCurve(hdc, c, g_graph->YMin(), g_graph->YMax());
            }

            DrawYLabels(hdc, g_graph->YMin(), g_graph->YMax());

            if (!g_graph->Curves().empty())
            {
                int n = (int)g_graph->Curves()[0].Size();
                DrawXLabels(hdc, n, g_graph->XStep());
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

Graph::Graph(const std::string title,
             const std::vector<Curve> curves,
             double yMin, double yMax, double step)
    : m_title(title),
      m_curves(curves),
      m_yMin(yMin),
      m_yMax(yMax),
      m_xStep(step)
{
}

const std::vector<Curve>& Graph::Curves() const { return m_curves; }
double Graph::YMin() const { return m_yMin; }
double Graph::YMax() const { return m_yMax; }
double Graph::XStep() const { return m_xStep; }
const std::string& Graph::Title() const { return m_title; }

void Graph::Show() const
{
    g_graph = this;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSA wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "KalmanGraphWindow";
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        "KalmanGraphWindow",
        m_title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        kWindowWidth, kWindowHeight,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}