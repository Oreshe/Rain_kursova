#include <windows.h> 
#include <vector> 
#include <algorithm> 

// Структура, яка представляє краплю 
struct Drop {
    int x, y;              // Центр краплі 
    float radius;          // Поточний радіус 
    int life;              // Час життя (кадри), після чого крапля зникає 
};

std::vector<Drop> drops;   // Всі активні краплі, що зараз відображаються 

const int TIMER_ID = 1;
const int FPS = 60;        // Частота оновлення екрана — 60 кадрів/с 

// Основна функція обробки повідомлень Windows 
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        // Старт таймера, який генерує повідомлення WM_TIMER з FPS частотою
            SetTimer(hwnd, TIMER_ID, 1000 / FPS, NULL);
        break;

    case WM_TIMER:
        // Оновлюємо стан кожної краплі (збільшуємо радіус, зменшуємо "життя")
        for (auto& drop : drops) {
            drop.radius += 2.0f;
            drop.life--;
        }
        // Видаляємо краплі, які "померли" (life <= 0) 
        drops.erase(std::remove_if(drops.begin(), drops.end(), [](Drop& d) {
            return d.life <= 0;
            }), drops.end());
        // Просимо ОС перемалювати вікно 
        InvalidateRect(hwnd, NULL, FALSE);
        break;

    case WM_LBUTTONDOWN: {
        // Обробка кліку миші — створення нової краплі 
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        drops.push_back({ x, y, 0.0f, 30 }); // Крапля з початковим радіусом 0 і життям 30 кадрів
            break;
    }

    case WM_KEYDOWN:
        // Вихід з програми по клавіші Escape 
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_PAINT: {
        // Малюємо всі краплі 
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Заповнюємо фон білим (колір системного вікна) 
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        // Створюємо синій контур (ручка) для крапель 
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 100, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH)); // Без заливки 

        // Малюємо кожну краплю як еліпс 
        for (const Drop& d : drops) {
            Ellipse(hdc, d.x - (int)d.radius, d.y - (int)d.radius,
                d.x + (int)d.radius, d.y + (int)d.radius);
        }

        // Повертаємо стару ручку та звільняємо ресурси 
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        // Завершення роботи: зупинка таймера, вихід із циклу повідомлень
        KillTimer(hwnd, TIMER_ID);
        PostQuitMessage(0);
        break;

    default:
        // Обробка інших повідомлень стандартним способом 
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int
    nCmdShow) {
    LPCWSTR className = L"RainWindow";

    // Реєстрація класу вікна 
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;     // Функція обробки повідомлень 
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;         // Ми малюємо фон вручну у 
    WM_PAINT;

        if (!RegisterClass(&wc)) {
            MessageBox(NULL, L"Не вдалося зареєструвати клас вікна",
                L"Помилка", MB_OK);
            return 1;
        }
    // Отримуємо розміри екрану для повноекранного режиму 
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Створення повноекранного, безрамкового, прозорого вікна 
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST,        // Поверх інших вікон 
        className,
        L"Rain",
        WS_POPUP,             // Без рамки та заголовка 
        0, 0, screenWidth, screenHeight,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, L"Не вдалося створити вікно", L"Помилка",
            MB_OK);
        return 1;
    }

    // Увімкнення прозорості для вікна 
    SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, 0, 100, LWA_ALPHA); // 100 із 255 — напівпрозорість

        // Показуємо та оновлюємо вікно 
        ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    // Головний цикл повідомлень програми 
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}