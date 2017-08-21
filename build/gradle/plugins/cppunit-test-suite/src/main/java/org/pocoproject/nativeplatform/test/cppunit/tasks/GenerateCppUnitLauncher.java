/*
 * Copyright 2014 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.pocoproject.nativeplatform.test.cppunit.tasks;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

import org.gradle.api.DefaultTask;
import org.gradle.api.UncheckedIOException;
import org.gradle.api.tasks.OutputDirectory;
import org.gradle.api.tasks.TaskAction;
import org.gradle.util.GFileUtils;
import org.pocoproject.nativeplatform.test.cppunit.CppUnitTestSuiteSpec;

/**
 * Generated the Gradle CPPUnit launcher: main method and header.
 */
public class GenerateCppUnitLauncher extends DefaultTask {
    private File sourceDir;
    private File headerDir;
    private CppUnitTestSuiteSpec cppUnitTestSuiteSpec;
    
    @TaskAction
    public void generate() {
        File source = new File(sourceDir, "gradle_cppunit_main.cpp");
        try {
            PrintWriter printWriter= new PrintWriter(source);
            printWriter.println("#include \"Poco/CppUnit/TestRunner.h\"");
            printWriter.print("#include \"");
            printWriter.print(cppUnitTestSuiteSpec.getName());
            printWriter.println(".h\"");
            printWriter.print("CppUnitMain(");
            printWriter.print(cppUnitTestSuiteSpec.getName());
            printWriter.println(")");
            printWriter.close();
        } catch (FileNotFoundException e) {
            throw new UncheckedIOException(e);
        }
    }

    private void writeToFile(File directory, String fileName) {
        final File file = new File(directory, fileName);
        GFileUtils.copyURLToFile(getClass().getResource(fileName), file);
    }

    @OutputDirectory
    public File getSourceDir() {
        return sourceDir;
    }

    public void setSourceDir(File sourceDir) {
        this.sourceDir = sourceDir;
    }

    @OutputDirectory
    public File getHeaderDir() {
        return headerDir;
    }

    public void setHeaderDir(File headerDir) {
        this.headerDir = headerDir;
    }
    
    public void setCppUnitTestSuiteSpec(CppUnitTestSuiteSpec cppUnitTestSuiteSpec) {
        this.cppUnitTestSuiteSpec = cppUnitTestSuiteSpec;
    }
}
