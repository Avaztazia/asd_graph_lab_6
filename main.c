#include<windows.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define numb 11
#define oneLen 180

int nx[numb] = {};
int ny[numb] = {};
double** mat = NULL;
double** tree = NULL;
int visited[numb] = {};
int thisOne = -1;
int check = 0;

int centerC ( int coord1, int coord2 )
{
    int res;
    if ( coord1 >= coord2 ) res = coord1 - (coord1 - coord2)/2;
    else res = coord2 - (coord2 - coord1)/2;
    return res;
}

void pausa ( HWND* hWnd )
{
    printf("\nClick to continue\n");
    while (1)
    {
		if (_kbhit()) {
			char c = _getch();
			InvalidateRect(*hWnd, NULL, TRUE);
			break;
		}
	}
}

double** randmm(int rows, int cols)
{
    double** matrix = (double**)malloc(rows * sizeof(double*));

    for (int i = 0; i < rows; i++)
        matrix[i] = (double*)malloc(cols * sizeof(double));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] =  2.0 / RAND_MAX * rand();
        }
    }

    return matrix;
}

double** mulmr(double num, double **mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat[i][j] = mat[i][j] * num;

            if(mat[i][j] > 1.0)
            {
                mat[i][j] = 1;
            } else mat[i][j] = 0;
        }
    }

    return mat;
}

void drawGraph ( HDC hdc, double** matrix )
{
    void arrow(float fi, int px,int py){
            fi = 3.1416*(180.0 - fi)/180.0;
            int lx,ly,rx,ry;
            lx = px+15*cos(fi+0.3);
            rx = px+15*cos(fi-0.3);
            ly = py+15*sin(fi+0.3);
            ry = py+15*sin(fi-0.3);
            MoveToEx(hdc, lx, ly, NULL);
            LineTo(hdc, px, py);
            LineTo(hdc, rx, ry);
    }
    char *nn[numb] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b"};
    int dx = 16, dy = 16, dtx = 7;
    int i;
    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
    HPEN current = CreatePen(PS_SOLID, 1, RGB(255, 250, 48));
    HPEN finished = CreatePen(PS_SOLID, 1, RGB(128, 255, 48));
    SelectObject(hdc, KPen);
    for (int i = 0; i < numb; i++)
            {
                for (int j = 0; j < numb; j++)
                {
                    if ( matrix[i][j] && visited[i]) {
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        if ( ( ( nx[i] == nx[j] ) && ( ( ny[i] - ny[j] == oneLen ) || ( ny[j] - ny[i] == oneLen ) ) ) != 1 && ( ( ny[i] == ny[j] ) && ( ( nx[i] - nx[j] == oneLen ) || ( nx[j] - nx[i] == oneLen ) ) ) != 1 && ( ( i == 0 && j == ( numb - 1 ) ) ||  ( j == 0 && i == ( numb - 1 ) ) ) != 1 ) {
                            if ( i != j ) {
                                POINT pt[3];
                                pt[0].x = nx[i];
                                pt[0].y = ny[i];
                                pt[2].x = nx[j];
                                pt[2].y = ny[j];

                                if ( nx[i] == nx[j] ){
                                    switch ( i - j ) {
                                    case 2: case -2:
                                        if ( nx[i] == oneLen ) {
                                            pt[0].x -= 15;
                                            pt[2].x -= 15;
                                            pt[1].x = nx[i] - oneLen/2;
                                            if ( ny[i] > ny[j] )
                                                pt[1].y = ny[i] - oneLen;
                                            else
                                                pt[1].y = ny[i] + oneLen;
                                        }
                                        else {
                                            pt[0].x += 15;
                                            pt[2].x += 15;
                                            pt[1].x = nx[i] + oneLen;
                                            if ( ny[i] > ny[j] )
                                                pt[1].y = ny[i] - oneLen;
                                            else
                                                pt[1].y = ny[i] + oneLen;
                                        }
                                        printWeight(hdc,matrix[i][j],pt[1].x,pt[1].y);
                                        Polyline(hdc,pt,3);
                                        break;
                                    default:
                                        printWeight(hdc,matrix[i][j],centerC(nx[i],nx[j]), centerC(ny[i], ny[j]));
                                        LineTo( hdc, nx[j], ny[j] );
                                        break;
                                    }
                                }
                                else if ( ny[i] == ny[j] ) {
                                    if ( ny[i] == oneLen || ny[i] == oneLen*3 ) {
                                        pt[1].y = ny[i];
                                        pt[1].x = nx[i];
                                        switch ( nx[i] - nx[j] ) {
                                        case 180*2: case 180*(-2):
                                            if ( ny[i] == oneLen ) {
                                                pt[1].y -= oneLen/2;
                                                if ( nx[i] > nx[j] )
                                                    pt[1].x -= oneLen;
                                                else
                                                    pt[1].x += oneLen;
                                            }
                                            else {
                                                pt[1].y += oneLen/2;
                                                if ( nx[i] > nx[j] )
                                                    pt[1].x -= oneLen;
                                                else
                                                    pt[1].x += oneLen;
                                            }
                                            break;
                                        case 180*3: case 180*(-3):
                                            if ( nx[i] > nx[j] ) {
                                                pt[0].x += 15;
                                                pt[2].x -= 15;
                                            }
                                            else {
                                                pt[0].x -= 15;
                                                pt[2].x += 15;
                                            }
                                            if ( ny[i] == oneLen ){
                                                pt[1].y -= oneLen/4*2;
                                                if ( nx[i] > nx[j] )
                                                    pt[1].x -= oneLen*3/2;
                                                else
                                                    pt[1].x += oneLen*3/2;
                                            }
                                            else {
                                                pt[1].y += oneLen/4*3;
                                                if ( nx[i] > nx[j] )
                                                    pt[1].x -= oneLen*3/2;
                                                else
                                                    pt[1].x += oneLen*3/2;
                                            }
                                            break;
                                        case 180*4: case 180*(-4):
                                            if ( nx[i] > nx[j] ) {
                                                pt[0].x += 13;
                                                pt[2].x -= 13;
                                            }
                                            else {
                                                pt[0].x -= 13;
                                                pt[2].x += 13;
                                            }
                                            if ( ny[i] == oneLen ) {
                                                pt[0].y -= 5;
                                                pt[2].y -= 5;
                                                pt[1].y -= oneLen;
                                                if ( nx[i] > nx[j] )
                                                    pt[1].x -= oneLen*2;
                                                else
                                                    pt[1].x += oneLen*2;
                                            }
                                            else {
                                                pt[0].y += 5;
                                                pt[2].y += 5;
                                                pt[1].y += oneLen*5/4;
                                                if ( nx[i] > nx[j] )
                                                    pt[1].x -= oneLen*2;
                                                else
                                                    pt[1].x += oneLen*2;\
                                            }
                                            break;
                                        }
                                        printWeight(hdc,matrix[i][j],pt[1].x,pt[1].y);
                                        Polyline(hdc,pt,3);
                                    }
                                    else {
                                        printWeight(hdc,matrix[i][j],centerC(nx[i],nx[j]), centerC(ny[i], ny[j]));
                                        LineTo( hdc, nx[j], ny[j] );
                                    }

                                }
                                else {
                                    if ( matrix[j][i] )
                                        if ( i - j < 0 ) {
                                            pt[1].x = nx[i] + 20;
                                            pt[1].y = ny[i] - oneLen;
                                            LineTo( hdc, nx[j], ny[j] );
                                        }
                                    else{
                                        printWeight(hdc,matrix[i][j],centerC(nx[i],nx[j]), centerC(ny[i], ny[j]));
                                        LineTo( hdc, nx[j], ny[j] );
                                    }
                                }
                            }
                        }
                        else {
                            printWeight(hdc,matrix[i][j],centerC(nx[i],nx[j]), centerC(ny[i], ny[j]));
                            LineTo( hdc, nx[j], ny[j] );
                        }

                    }
                }
            }

            for ( i = 0; i <= 10; i++ ){
            if ( !check && i == thisOne ) SelectObject(hdc, current);
            else if ( !check && visited[i] ) SelectObject(hdc, finished);
            else SelectObject(hdc, BPen);
              Ellipse ( hdc, nx[i]-dx, ny[i]-dy, nx[i]+dx, ny[i]+dy );
              TextOut ( hdc, nx[i]-dtx, ny[i]-dy/2, nn[i], 1 );
            }
}

void printMatrix ( double** matrix, int rows, int cols ) {
    for ( int i = 0; i < rows; i++ ){
        for ( int j = 0; j < cols; j++ )
            printf("%.0f\t", matrix[i][j]);
        printf("\n");
    }
}

void printWeight ( HDC hdc, double weight, int x, int y )
{
    int len;
    int numbe = weight;
    if ( numbe < 10 ) len = 1;
    else if ( numbe < 100 ) len = 2;
    else if ( numbe < 1000 ) len = 3;
    else len = 4;
    char* buf;
    buf = (char *)malloc(10 * sizeof(char));
    int v = 0;
    while ( numbe > 9 ) {
        buf[v++] = (numbe % 10) + '0';
        numbe = numbe / 10;
    }
    buf[v++] = numbe + '0';
    buf[v] = '\0';
    char t;
    for (int i = 0; i < v / 2; i++)
    {
        t = buf[i];
        buf[i] = buf[v - 1 - i];
        buf[v - 1 - i] = t;
    }
    TextOut ( hdc, x, y, buf, len );
}

void generateMatrixes (  )
{
    for ( int i = 0; i < numb; i++ ) {
        if ( i == 0 ) {
            nx[i] = oneLen*3;
            ny[i] = oneLen;
        }
        else if ( i < 3) {
            nx[i] = nx[i-1] - oneLen;
            ny[i] = ny[i-1];
        }
        else if ( i < 5 ) {
            nx[i] = nx[i-1];
            ny[i] = ny[i-1] + oneLen;
        }
        else if ( i < 9 ){
            nx[i] = nx[i-1] + oneLen;
            ny[i] = ny[i-1];
        }
        else {
            nx[i] = nx[i-1];
            ny[i] = ny[i-1] - oneLen;
        }
    }
    srand(0412);
    double** T = randmm(numb, numb);
    double** ran = randmm(numb, numb);
    double coef = 1.0 - 1*0.01 - 2*0.005 - 0.05;
    mat = mulmr(coef, T, numb, numb);
    tree = randmm(numb, numb);

    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            mat[i][j] = round ( mat[i][j] * ran[i][j] * 100 );
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ ) {
            int a = mat[i][j] == mat[j][i] && mat[i][j] == 1;
            int b = mat[i][j] != mat[j][i];
            if ( j > i ) a = a * ( mat[i][j] > 0 );
            mat[i][j] = mat[i][j] * ( a + b );
        }
    for ( int i = 0; i < numb; i++ )
        for ( int j = 0; j < numb; j++ )
            mat[j][i] = mat[i][j];
    printf("\nMatrix:\n");
    printMatrix ( mat, numb, numb );
}


void checker ( HDC hdc )
{
    if ( !check ) drawGraph( hdc, mat );
    else drawGraph( hdc, tree );
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


DWORD WINAPI secondfun (LPVOID lpParam)                                                             //second
{
    generateMatrixes();
    for ( int i = 0; i < numb; i++ ) {
        visited[i] = 0;
        for ( int j = 0; j < numb; j++ )
            tree[i][j] = 0;
    }
    visited[0] = 1;
    int minWeight = INT_MAX;
    int starter = -1;
    int ender = -1;
    for ( int k = 0; k < numb; k++ ) {
        starter = -1;
        ender = -1;
        minWeight = INT_MAX;
        for ( int i = 0; i < numb; i++ )
            if ( !visited[i] )
                for ( int j = 0; j < numb; j++ )
                    if ( i != j && mat[i][j] && !visited[j] && mat[i][j] < minWeight ) {
                        starter = i;
                        ender = j;
                        minWeight = mat[i][j];
                    }
        if ( starter != -1 && ender != -1 ) {
            pausa( lpParam );
            tree[starter][ender] = minWeight;
            tree[ender][starter] = minWeight;
            visited[ender] = 1;
        }
    }
    printf("\nspanning tree matrix:\n");
    printMatrix(tree, numb, numb);
    printf("\nswitch to spanning tree");
    pausa( lpParam );
    check = 1;
}


char ProgName[]="Lab 6";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hWnd;
    MSG lpMsg;
    WNDCLASS w;
    w.lpszClassName=ProgName;
    w.hInstance=hInstance;
    w.lpfnWndProc=WndProc;
    w.hCursor=LoadCursor(NULL, IDC_ARROW);
    w.hIcon=0;
    w.lpszMenuName=0;
    w.hbrBackground = LTGRAY_BRUSH;
    w.style=CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra=0;
    w.cbWndExtra=0;
    if(!RegisterClass(&w))
        return 0;
    hWnd=CreateWindow(ProgName,
        "Lab 6",
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        1920,
        1080,
        (HWND)NULL,
        (HMENU)NULL,
        (HINSTANCE)hInstance,
        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);

    DWORD dwThreadId = 1;
    HANDLE secpot;
    secpot = CreateThread(NULL, 0, secondfun, &hWnd, 0, &dwThreadId);

    while(GetMessage(&lpMsg, hWnd, 0, 0)) {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
    return(lpMsg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT messg,
                        WPARAM wParam, LPARAM lParam)
    {
    HDC hdc;
    PAINTSTRUCT ps;
    switch(messg){
        case WM_PAINT :

            hdc=BeginPaint(hWnd, &ps);
            checker ( hdc );
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return(DefWindowProc(hWnd, messg, wParam, lParam));
    }
    return 0;
}
