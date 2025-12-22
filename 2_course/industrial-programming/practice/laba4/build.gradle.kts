// build.gradle.kts

plugins {
    // Применяем плагин Java для сборки Java-проектов
    java
    // Использование Application Plugin для создания исполняемого JAR и запуска из командной строки
    application
}

group = "org.example" // Измените на подходящее имя группы
version = "1.0-SNAPSHOT" // Версия вашего проекта

// Конфигурация репозиториев для поиска зависимостей
repositories {
    // Используем Maven Central для стандартных библиотек
    mavenCentral()
}

// Конфигурация задачи 'java'
java {
    // Указываем, что исходный код написан для Java 8 или более новой версии (например, 17)
    // Измените это значение на версию, которую вы предпочитаете использовать
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
}

// Конфигурация Application Plugin
application {
    mainClass.set("org.example.Main")
}

dependencies {
    // Пример добавления зависимости для тестов (JUnit 5)
    // testImplementation("org.junit.jupiter:junit-jupiter-api:5.10.0")
    // testRuntimeOnly("org.junit.jupiter:junit-jupiter-engine:5.10.0")
}

// Убедимся, что кодировка при компиляции всегда UTF-8
tasks.withType<JavaCompile> {
    options.encoding = "UTF-8"
}