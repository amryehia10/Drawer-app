#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
#include<iostream>
using namespace std;
class Vector2
{
public:
    double x,y;
    Vector2()
    {
        x=0;
        y=0;
    }

    Vector2(double a,double b)
    {
        x=a; y=b;

    }
};
class Vector4

{

    double v[4];
public:


    Vector4(double a=0,double b=0,double c=0,double d=0)
    {
        v[0]=a; v[1]=b; v[2]=c; v[3]=d;
    }


    Vector4(double a[])
    {
        memcpy(v,a,4*sizeof(double));
    }


    double& operator[](int i)
    {
        return v[i];
    }


};



class Matrix4
{
    Vector4 M[4];
public:
    Matrix4(double A[])
    {
        memcpy(M,A,16*sizeof(double));
    }
    Vector4& operator[](int i)
    {
        return M[i];
    }
};


class curve{

public :
    Vector2 p0;
    Vector2 p1;
    Vector2 p2;
    Vector2 p3;
    curve()
    {

    }

    curve(Vector2 p_1,Vector2 p_2,Vector2 p_3,Vector2 p_4)
    {
        p0.x=p_1.x;
        p0.y=p_1.y;
        p1.x=p_2.x;
        p1.y=p_2.y;
        p2.x=p_3.x;
        p2.y=p_3.y;
        p3.x=p_4.x;
        p3.y=p_4.y;

    }

/*--------------------------------------------------------------------------------------------*/
    curve &operator =(const curve& other){
        p0.x=other.p0.x;
        p0.y=other.p0.y;
        p1.x=other.p1.x;
        p1.y=other.p1.y;
        p2.x=other.p2.x;
        p2.y=other.p2.y;
        p3.x=other.p3.x;
        p3.y=other.p3.y;
    }

    void draw_curve(HDC hdc,COLORREF c){
        DrawHermiteCurve(hdc,p0,p1,p2,p3,1000,c);
    }




    double DotProduct(Vector4& a,Vector4& b) //multiplying a raw vector by a column vector
    {
        return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3];
    }

/*--------------------------------------------------------------------------------------------*/

    Vector4 multi(Matrix4 M,Vector4& b) // right multiplication of M by b
    {
        Vector4 res;
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                res[i]+=M[i][j]*b[j];
        return res;
    }



/*--------------------------------------------------------------------------------------------*/


    Vector4 GetHermiteCoeff(double x0,double s0,double x1,double s1)
    {
        static double H[16]={2,1,-2,1,-3,-2,3,-1,0,1,0,0,1,0,0,0};
        static Matrix4 basis(H);
        Vector4 v(x0,s0,x1,s1);
        return multi(basis,v);
    }

/*-----------------------*/


    void DrawHermiteCurve (HDC hdc,Vector2& P0,Vector2& T0,Vector2& P1,Vector2& T1 ,int numpoints, COLORREF c)
    {
        Vector4 xcoeff=GetHermiteCoeff(P0.x,T0.x,P1.x,T1.x);
        Vector4 ycoeff=GetHermiteCoeff(P0.y,T0.y,P1.y,T1.y);
        if(numpoints<2)return;
        double dt=1.0/(numpoints-1);
        for(double t=0;t<=1;t+=dt)
        {
            Vector4 vt;
            vt[3]=1;
            for(int i=2;i>=0;i--)vt[i]=vt[i+1]*t;
            int x=round(DotProduct(xcoeff,vt));
            int y=round(DotProduct(ycoeff,vt));
            SetPixel(hdc,x,y,c);
        }

    }

/*--------------------------------------------------------------------------------------------*/


};



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
HMENU hmenu;
HMENU bmenu;
HWND g_hWnd;
/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOWFRAME;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
            0,                   /* Extended possibilites for variation */
            szClassName,         /* Classname */
            _T("Code::Blocks Template Windows App"),       /* Title Text */
            WS_OVERLAPPEDWINDOW, /* default window */
            CW_USEDEFAULT,       /* Windows decides the position */
            CW_USEDEFAULT,       /* where the window ends up on the screen */
            1920,                 /* The programs width */
            800,                 /* and height in pixels */
            HWND_DESKTOP,        /* The window is a child-window to desktop */
            NULL,                /* No menu */
            hThisInstance,       /* Program Instance handler */
            NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
int Round(double x)
{
    return (int)(0.5+x);
}
void DDALine(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    double dx=x2-x1;
    double dy=y2-y1;
    if(abs(dx) >= abs(dy))
    {
        if(x2 < x1){
            swap(x1, x2);
            swap(y1 , y2);
        }
        SetPixel(hdc,x1,y1,color);
        double X=x1;
        double m = dy / dx;
        double Y = y1;
        while(X < x2)
        {
            X++;
            Y += m;
            SetPixel(hdc,X,Round(Y),color);
        }
    }
    else
    {
        if(y2 < y1){
            swap(x1, x2);
            swap(y1 , y2);
        }
        SetPixel(hdc, x1, y1, color);
        double Y = y1;
        double X = x1;
        double m = dx / dy;
        while(Y < y2)
        {
            Y++;
            X += m;
            SetPixel(hdc,Round(X),Y,color);
        }
    }
}
void drawPoint(HDC hdc,int xc,int yc,int x,int y,COLORREF c)
{
    SetPixel(hdc,xc+x,yc+y,c);
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc-x,yc-y,c);
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc+y,yc+x,c);
    SetPixel(hdc,xc-y,yc+x,c);
    SetPixel(hdc,xc-y,yc-x,c);
    SetPixel(hdc,xc+y,yc-x,c);
}
void midpointCircle(HDC hdc,int xc,int yc,int r,COLORREF C)
{
    /**
    circleEqu=x^2+y^2-r^2
    dintial(x+1,y-0.5)=(5/4)-r == 1-r
    d1(x+2,y-0.5)=2x+3  if(d<0) inside circle x++ y=constant
    d2(x+2,y-1.5)=2(x-y)+5 if(d>0) outside circle  x++ y--
    */
    double y=r,x=0,d=1-r;
    drawPoint(hdc,xc,yc,Round(x),Round(y),C);
    while(x<=y)
    {
        if(d<0)
        {
            d+=2*x+3;
            x++;
        }
        else
        {
            d+=2*(x-y)+5;
            x++;
            y--;
        }
        drawPoint(hdc,xc,yc,Round(x),Round(y),C);
    }
}

union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};


OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}


void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}


void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while((out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {
        MoveToEx(hdc,Round(x1),Round(y1),NULL);
        LineTo(hdc,Round(x2),Round(y2));
    }
}
//Mahmoud
void SplitColor(HDC hdc, int x, int y, int xc, int yc, int r, COLORREF c){
   int d = r * r - ((xc-x)*(xc-x) + (yc-y)*(yc-y));
   if(d>0){
        for(double t = 0; t <= 1; t+= 0.0000001){
            if(c = 'red')
                SetPixel(hdc,x,y,c);
            else if(c == 'green')
                SetPixel(hdc,x,y,c);
            else
                SetPixel(hdc,x,y,c);
        }
   }
   else{
       for(double t = 0; t <= 1; t+= 0.0000001){
           if(c = 'red')
               SetPixel(hdc,x,y, RGB(0,0,255));
           else if(c == 'green')
               SetPixel(hdc,x,y,RGB(255,0,0));
           else
               SetPixel(hdc,x,y,RGB(0,255,0));
       }
   }

}



/*  This function is called by the Windows function DispatchMessage()  */
double x_0,y_0,s_0_x, s_0_y, x_1, y_1, s_1_x, s_1_y;
Vector2 P_0,T_0,P_1,T_1;
int count =1;
curve c;
int X1,Y1,x2,y2,xc,yc,y,x,Y,k,nume,R,R2,xf,yf,match,x1,y3,xl,yt,xr,yb,xf2,yf2;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_COMMAND:      //sent when a user selects a command item from a menu
            switch (wParam)   //pass things like handles and integers
            {
                case 'one':  //shape color red
                    Y = RGB(255, 0, 0);
                    break;
                case 'two':  //shape color blue
                    Y = RGB(0, 0, 255);
                    break;
                case 'three':  //shape color green
                    Y = RGB(0, 255, 0);
                    break;
                case 'seven':
                    match = 1;
                    break;

                case 'MidpointCircle':
                    match = 3;
                    break;

                case 'DDA':
                    match = 5;
                    break;

                case 'clipLin':
                    match = 8;
                    break;

                case 'curve':
                    match = 10;
                    break;
            }
        case WM_LBUTTONDBLCLK:
            xl = LOWORD(lParam);   //pass pointers
            yt = HIWORD(lParam);
            break;
        case WM_RBUTTONDBLCLK:
            xr = LOWORD(lParam);
            yb = HIWORD(lParam);
            xf = xl;
            yf = yt;
            xf2 = xr;
            yf2 = yb;
            break;
        case WM_LBUTTONDOWN:
            xc = LOWORD(lParam);
            yc = HIWORD(lParam);
            X1 = LOWORD(lParam);
            Y1 = HIWORD(lParam);
            break;

        case WM_RBUTTONDOWN:
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            R = Round(sqrt(pow(x2 - xc, 2.0) + pow(y2 - yc, 2.0)));

            if (match == 3) {
                midpointCircle(hdc, xc, yc, R, Y);
            }

            if (match == 5) {
                DDALine(hdc, X1, Y1, x2, y2, Y);
            }

            if (match == 8) {
                CohenSuth(hdc, X1, Y1, x2, y2, xf, yf, xf2, yf2);
            }
            break;

        case WM_LBUTTONUP:
            if(match == 10) {
                if (count == 1) {
                    x_0 = LOWORD(lParam);
                    y_0 = HIWORD(lParam);
                    P_0.x = x_0;
                    P_0.y = y_0;
                    count++;
                } else if (count == 2) {
                    s_0_x = LOWORD(lParam);
                    s_0_y = HIWORD(lParam);
                    T_0.x = s_0_x;
                    T_0.y = s_0_y;
                    count++;
                } else if (count == 3) {
                    s_1_x = LOWORD(lParam);
                    s_1_y = HIWORD(lParam);
                    T_1.x = s_1_x;
                    T_1.y = s_1_y;
                    count++;
                } else {
                    x_1 = LOWORD(lParam);
                    y_1 = HIWORD(lParam);
                    P_1.x = x_1;
                    P_1.y = y_1;
                    c = curve(P_0, T_0, P_1, T_1);
                    c.draw_curve(hdc,Y);
                    //DrawHermiteCurve(hdc,P_0,T_0,P_1,T_1,1000);
                    count = 1;
                }
            }


                //menu
                case WM_CREATE:
                    AddMenus(hwnd);
                break;
                default:                      /* for messages that we don't deal with */
                    return DefWindowProc(hwnd, message, wParam, lParam);
            }
            return 0;
    }


void AddMenus(HWND hwnd) {

    hmenu = CreateMenu();

    HMENU hcolor = CreateMenu();

    HMENU circle = CreateMenu();

    HMENU line = CreateMenu();

    HMENU clipping = CreateMenu();

    HMENU curve = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) hcolor, "Shape Color");

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) circle, "Circle");

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) line, "Line");

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR) clipping, "Clipping");

    AppendMenu(hmenu,MF_POPUP,(UINT_PTR)curve,"Curve");

    AppendMenu(hcolor, MF_STRING, 'one', "red");
    AppendMenu(hcolor, MF_STRING, 'two', "blue");
    AppendMenu(hcolor, MF_STRING, 'three', "green");

    AppendMenu(circle, MF_STRING, 'MidpointCircle', "Midpoint");

    AppendMenu(line, MF_STRING, 'DDA', "DDA");

    AppendMenu(clipping, MF_STRING, 'clipLin', "Line");

    AppendMenu(curve, MF_POPUP,  'Curve' , "Hermite");
    SetMenu(hwnd, hmenu);
}