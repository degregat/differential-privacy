#  Copyright 2021 Cyrus Katrak
#  Copyright 2021 Slack Technologies
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#  Source: https://github.com/slackhq/grpc-hack/blob/master/cc_library_static.bzl


def _cc_static_library_impl(ctx):
    cc = ctx.attr.dep[CcInfo]
    libraries = []
    for link_input in cc.linking_context.linker_inputs.to_list():
        for library in link_input.libraries:
            libraries += library.pic_objects
    args = ["r", ctx.outputs.out.path] + [f.path for f in libraries]
    ctx.actions.run(
        inputs = libraries,
        outputs = [ctx.outputs.out],
        executable = "/usr/bin/ar",
        arguments = args,
    )
    return [DefaultInfo()]

cc_static_library = rule(
    implementation = _cc_static_library_impl,
    attrs = {
        "dep": attr.label(providers = [CcInfo]),
    },
    outputs = {"out": "%{name}.a"},
)
