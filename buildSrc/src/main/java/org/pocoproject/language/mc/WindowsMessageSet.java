/*
 * Copyright 2013 the original author or authors.
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
package org.pocoproject.language.mc;

import org.gradle.api.Incubating;
import org.gradle.language.nativeplatform.HeaderExportingSourceSet;
import org.pocoproject.language.nativeplatform.NativeMessageSet;
import org.gradle.language.base.LanguageSourceSet;

/**
 * A set of Windows Message definition files.
 *
 * <p>A Windows Message set contains a set of message files./p>
 *
 * <pre autoTested="true">
 * apply plugin: "windows-messages"
 *
 * model {
 *     components {
 *         main(NativeLibrarySpec) {
 *             sources {
 *                 mc {
 *                     source {
 *                         srcDirs "src/main/mc"
 *                         include "**{@literal /}*.mc"
 *                     }
 *                     exportedHeaders {
 *                         srcDirs "src/main/include"
 *                     }
 *                 }
 *             }
 *         }
 *     }
 * }
 * </pre>
 */
@Incubating
public interface WindowsMessageSet extends LanguageSourceSet, HeaderExportingSourceSet, NativeMessageSet {
}
