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
package org.pocoproject.language.mc.plugins;

import com.google.common.collect.Maps;
import org.gradle.api.Incubating;
import org.gradle.api.Plugin;
import org.gradle.api.Project;
import org.gradle.internal.service.ServiceRegistry;
import org.gradle.language.base.internal.SourceTransformTaskConfig;
import org.gradle.language.base.internal.registry.LanguageTransformContainer;
import org.gradle.language.base.plugins.ComponentModelBasePlugin;
import org.gradle.language.nativeplatform.internal.NativeLanguageTransform;
import org.gradle.model.Mutate;
import org.gradle.model.RuleSource;
import org.gradle.nativeplatform.NativeBinarySpec;
import org.gradle.nativeplatform.internal.DefaultPreprocessingTool;
import org.gradle.platform.base.BinarySpec;
import org.gradle.platform.base.ComponentType;
import org.gradle.platform.base.TypeBuilder;
import org.pocoproject.language.mc.WindowsMessageSet;
import org.pocoproject.language.mc.internal.DefaultWindowsMessageSet;
import org.pocoproject.language.mc.plugins.internal.WindowsMessagesCompileTaskConfig;

import java.util.Map;

/**
 * Adds core language support for Windows resource script files.
 */
@Incubating
public class WindowsMessageScriptPlugin implements Plugin<Project> {

    @Override
    public void apply(final Project project) {
        project.getPluginManager().apply(ComponentModelBasePlugin.class);
    }

    @SuppressWarnings("UnusedDeclaration")
    static class Rules extends RuleSource {
        @ComponentType
        void registerLanguage(TypeBuilder<WindowsMessageSet> builder) {
            builder.defaultImplementation(DefaultWindowsMessageSet.class);
        }

        @Mutate
        void registerLanguageTransform(LanguageTransformContainer languages, ServiceRegistry serviceRegistry) {
            languages.add(new WindowsMessages());
        }
    }

    private static class WindowsMessages extends NativeLanguageTransform<WindowsMessageSet> {
        @Override
        public Class<WindowsMessageSet> getSourceSetType() {
            return WindowsMessageSet.class;
        }

        @Override
        public Map<String, Class<?>> getBinaryTools() {
            Map<String, Class<?>> tools = Maps.newLinkedHashMap();
            tools.put("mcCompiler", DefaultPreprocessingTool.class);
            return tools;
        }

        @Override
        public String getLanguageName() {
            return "mc";
        }

        @Override
        public SourceTransformTaskConfig getTransformTask() {
            return new WindowsMessagesCompileTaskConfig();
        }

        @Override
        public boolean applyToBinary(BinarySpec binary) {
            return binary instanceof NativeBinarySpec && shouldProcessMessages((NativeBinarySpec) binary);
        }

        private boolean shouldProcessMessages(NativeBinarySpec binary) {
            return binary.getTargetPlatform().getOperatingSystem().isWindows();
        }
    }
}
