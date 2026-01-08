import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

plugins {
    id("org.springframework.boot") version "3.2.2" // Актуальная версия Spring Boot
    id("io.spring.dependency-management") version "1.1.4"
    kotlin("jvm") version "1.9.22"
    kotlin("plugin.spring") version "1.9.22" // Позволяет Spring работать с final-классами Kotlin
}

group = "com.example"
version = "0.0.1-SNAPSHOT"

java {
    sourceCompatibility = JavaVersion.VERSION_17 // Или VERSION_21, если скачали её
}

repositories {
    mavenCentral() // Откуда скачивать библиотеки
}

dependencies {
    // Основная зависимость для REST API
    implementation("org.springframework.boot:spring-boot-starter-web")

    // Поддержка Kotlin
    implementation("com.fasterxml.jackson.module:jackson-module-kotlin")
    implementation("org.jetbrains.kotlin:kotlin-reflect")

    // Библиотеки для тестирования
    testImplementation("org.springframework.boot:spring-boot-starter-test")
}

tasks.withType<KotlinCompile> {
    kotlinOptions {
        freeCompilerArgs += "-Xjsr305=strict" // Улучшает совместимость с Java Nullability
        jvmTarget = "17"
    }
}

tasks.withType<Test> {
    useJUnitPlatform() // Обязательно для работы JUnit 5
}