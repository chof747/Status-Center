Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
# print(defines)

env.Replace(PROGNAME="%s_%s" % (defines.get("PROGNAME"), defines.get("VERSION").replace('"', '')))