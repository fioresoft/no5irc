function GetScriptEngineInfo()
{
    var s;
    s = ""; // Build string with necessary info.
    s += ScriptEngine() + " Version ";
    s += ScriptEngineMajorVersion() + ".";
    s += ScriptEngineMinorVersion() + ".";
    s += ScriptEngineBuildVersion();
    return(s);
}

no5.Output(GetScriptEngineInfo());