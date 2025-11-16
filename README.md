
# C++ Logger

Лёгкий потокобезопасный логгер на C++ с потоковым API, уровнями логирования и удобными макросами вида:

```cpp
LOG_INFO << "Application started";
LOG_ERROR << "Something bad happened";
```

---

## Возможности

* **Один логгер на всё приложение**
  Паттерн Singleton: `logger::Logger::instance()`
  Конструктор приватный, копирование/перемещение запрещены.

* **Потоковый интерфейс**
  Логирование выглядит как запись в `std::ostream`:

  ```cpp
  logger::Logger::instance().info() << "User logged in: " << user.name;
  ```

* **Уровни логирования**

  ```cpp
  enum class LogLevel {
      Trace,
      Debug,
      Info,
      Warn,
      Error,
      Fatal
  };
  ```

  Минимальный уровень задаёт “порог” логирования: сообщения ниже порога игнорируются.

* **Настройка через переменную окружения**
  Уровень логирования можно задать переменной:

  ```bash
  export LOG_LEVEL=DEBUG
  ```

  Поддерживаемые значения (регистр не важен):
  `TRACE`, `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`.

* **Потокобезопасность**

  * Доступ к выходному потоку защищён `std::mutex`.
  * Текущий уровень логирования хранится в `std::atomic<LogLevel>`.

* **Удобные макросы для логирования**

  ```cpp
  LOG_TRACE << "trace details";
  LOG_DEBUG << "debug data";
  LOG_INFO  << "something happened";
  LOG_WARN  << "suspicious situation";
  LOG_ERROR << "error text";
  LOG_FATAL << "fatal error";
  ```

  Каждый макрос автоматически добавляет контекст:

  ```text
  [Info] [main.cpp:42 main] Application started
  ```

  где:

  * имя файла — `__FILE__`,
  * номер строки — `__LINE__`,
  * имя функции — `__func__`.

---

## Структура

Основные файлы:

* `Logger.hpp` — интерфейс логгера (`logger::Logger`, `logger::LogLevel`, `logger::LogStream`).
* `Logger.cpp` — реализация логгера.
* `LoggerMacros.hpp` — макросы `LOG_TRACE`, `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`, `LOG_FATAL`.

---

## Подключение в проект

1. Добавь исходники в проект.

2. В нужном `.cpp` файле подключи:

   ```cpp
   #include "LoggerMacros.hpp"
   ```

3. При необходимости, на старте программы настрой уровень логирования из окружения:

   ```cpp
   logger::Logger::instance().load_env_level();
   ```

   Если переменной окружения `LOG_LEVEL` нет, используется уровень по умолчанию (`Info`).

---

## Примеры использования

### Базовое логирование

```cpp
void run() {
    LOG_INFO  << "Application started";
    LOG_DEBUG << "Config loaded";
    LOG_WARN  << "Low disk space";
    LOG_ERROR << "Failed to open file";
}
```

### Логирование с данными

```cpp
void handle_request(int id, std::string_view user) {
    LOG_INFO << "Handle request: id=" << id << ", user=" << user;
}
```

Каждая запись будет содержать:

* уровень (`[Info]`, `[Error]`, …);
* файл, строку и имя функции;
* ваше сообщение.

---

## Настройка уровня логирования

Уровень по умолчанию — `Info`.
Его можно изменить:

### Через переменную окружения

```bash
export LOG_LEVEL=TRACE
# или
set LOG_LEVEL=ERROR   # Windows
```

Поддерживаются значения (без учёта регистра):

* `TRACE`
* `DEBUG`
* `INFO`
* `WARN`
* `ERROR`
* `FATAL`

### Программно

Можно выставить уровень вручную:

```cpp
logger::Logger::instance().set_level(logger::LogLevel::Debug);
```

---

## Внутреннее устройство (кратко)

* `logger::Logger`:

  * хранит текущий `LogLevel`;
  * предоставляет методы `trace()`, `debug()`, `info()`, `warn()`, `error()`, `fatal()`;
  * реализует `write(LogLevel, const std::string&)`, который:

    * фильтрует по уровню;
    * форматирует префикс уровня (`[Info] ` и т.п.);
    * пишет в `std::clog` под мьютексом.

* `logger::LogStream`:

  * создаётся через один из методов `Logger`;
  * копит сообщение в `std::ostringstream`;
  * в деструкторе отдаёт строку в `Logger::write()`.

* Макросы `LOG_*`:

  * разворачиваются в вызов нужного метода логгера + префикс `[file:line func]`;
  * позволяют писать коротко: `LOG_INFO << "message";`.

---

## Требования

* C++17 или выше.
* Стандартная библиотека (`<atomic>`, `<mutex>`, `<sstream>`, `<string>`, `<iostream>`, `<cstdlib>`, `<algorithm>`, `<optional>` при необходимости).

