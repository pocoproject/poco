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
package org.pocoproject.language.mc.plugins.internal;

import org.gradle.api.DefaultTask;
import org.gradle.api.Project;
import org.gradle.api.Task;
import org.gradle.api.file.FileTree;
import org.gradle.api.tasks.util.PatternSet;
import org.gradle.internal.service.ServiceRegistry;
import org.gradle.language.base.LanguageSourceSet;
import org.gradle.language.base.internal.LanguageSourceSetInternal;
import org.gradle.language.base.internal.SourceTransformTaskConfig;
import org.gradle.nativeplatform.PreprocessingTool;
import org.gradle.nativeplatform.internal.NativeBinarySpecInternal;
import org.gradle.nativeplatform.internal.StaticLibraryBinarySpecInternal;
import org.gradle.platform.base.BinarySpec;
import org.pocoproject.language.mc.WindowsMessageSet;
import org.pocoproject.language.mc.tasks.WindowsMessageCompile;

import java.io.File;
import java.util.Set;
import java.util.concurrent.Callable;

public class WindowsMessagesCompileTaskConfig implements SourceTransformTaskConfig {
    @Override
    public String getTaskPrefix() {
        return "compile";
    }

    @Override
    public Class<? extends DefaultTask> getTaskType() {
        return WindowsMessageCompile.class;
    }

    @Override
    public void configureTask(Task task, BinarySpec binary, LanguageSourceSet sourceSet, ServiceRegistry serviceRegistry) {
        configureMessageCompileTask((WindowsMessageCompile) task, (NativeBinarySpecInternal) binary, (WindowsMessageSet) sourceSet);
    }

    private void configureMessageCompileTask(WindowsMessageCompile task, final NativeBinarySpecInternal binary, final WindowsMessageSet sourceSet) {
        task.setDescription("Compiles messages of the " + sourceSet + " of " + binary);

        task.setToolChain(binary.getToolChain());
        task.setTargetPlatform(binary.getTargetPlatform());

        task.includes(new Callable<Set<File>>() {
            public Set<File> call() {
                return sourceSet.getExportedHeaders().getSrcDirs();
            }
        });
        task.source(sourceSet.getSource());

        final Project project = task.getProject();
        task.setOutputDir(new File(binary.getNamingScheme().getOutputDirectory(project.getBuildDir(), "objs"), ((LanguageSourceSetInternal) sourceSet).getProjectScopedName()));

        PreprocessingTool mcCompiler = (PreprocessingTool) binary.getToolByName("mcCompiler");
        task.setMacros(mcCompiler.getMacros());
        task.setCompilerArgs(mcCompiler.getArgs());

        FileTree resourceOutputs = task.getOutputs().getFiles().getAsFileTree().matching(new PatternSet().include("**/*.msg"));
        binary.binaryInputs(resourceOutputs);
        if (binary instanceof StaticLibraryBinarySpecInternal) {
            ((StaticLibraryBinarySpecInternal) binary).additionalLinkFiles(resourceOutputs);
        }
    }

}
