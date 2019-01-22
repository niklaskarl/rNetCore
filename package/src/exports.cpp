#include "pal/path.h"
#include "clrhost/clrhosting.h"

#include <Rcpp.h>

RcppExport SEXP rnetcore_export_nwhich()
{
    try
    {
        utf16string directory = find_dotnet_core_absolute_directory();
        utf8string directory_u8 = utf16_to_utf8(directory);

        Rcpp::CharacterVector result(1);
        result[0] = directory_u8.c_str();
        return result;
    }
    catch (const std::exception &exc)
    {
        Rcpp::Rcerr << "dotnet directory not found on path.\n";
        Rcpp::Rcerr << exc.what() << "\n";
        return R_NilValue;
    }
}

RcppExport SEXP rnetcore_export_nversions(SEXP arg_directory)
{
    const utf8char_t *directory = Rcpp::CharacterVector(arg_directory)[0];
    utf16string directory_u16 = utf8_to_utf16(directory);

    try
    {
        std::vector<SemVer> versions = get_installed_dotnet_core_versions(directory_u16.c_str());
        Rcpp::CharacterVector result(versions.size());
        std::string version_text;
        for (size_t i = 0; i < versions.size(); i++)
        {
            std::string version_text = versions[i].ToString();
            result[i] = version_text.c_str();
        }

        return result;
    }
    catch (const std::exception &exc)
    {
        Rcpp::Rcerr << "unable to obtain installed versions of .NET Core.";
        Rcpp::Rcerr << exc.what() << "\n";
        return R_NilValue;
    }
}

RcppExport SEXP rnetcore_export_nload_coreclr(SEXP arg_directory, SEXP arg_version)
{
    const utf8char_t *directory = Rcpp::CharacterVector(arg_directory)[0];
    utf16string directory_u16 = utf8_to_utf16(directory);

    const utf8char_t *version_text = Rcpp::CharacterVector(arg_version)[0];

    SemVer version;
    if (!SemVer::TryParse(version_text, version))
    {
        Rcpp::Rcerr << "the specified version is not a valid semantic version.\n";
        return R_NilValue;
    }

    try
    {
        clr_host *host = new clr_host(load_dotnet_core_runtime(directory_u16.c_str(), version));
        return Rcpp::XPtr<clr_host>(host);
    }
    catch (const std::exception &exc)
    {
        Rcpp::Rcerr << "unable to load coreclr into process.\n";
        Rcpp::Rcerr << exc.what() << "\n";
        return R_NilValue;
    }
}

RcppExport SEXP rnetcore_export_ninit_app_domain(SEXP arg_host, SEXP arg_path)
{
    clr_host *host = Rcpp::XPtr<clr_host>(arg_host);
    const utf8char_t *path = Rcpp::CharacterVector(arg_path)[0];
    utf16string path_u16 = utf8_to_utf16(path);

    utf16string absolute_path;
    if (!pal::path::get_absolute_path(path_u16.c_str(), absolute_path))
    {
        Rcpp::Rcerr << "assembly file doesn't exist.\n";
        return R_NilValue;
    }

    try
    {
        clr_app_domain *domain = new clr_app_domain(host->create_app_domain(absolute_path.c_str()));
        return Rcpp::XPtr<clr_app_domain>(domain);
    }
    catch (const std::exception &exc)
    {
        Rcpp::Rcerr << "unable to initialize app domain.\n";
        Rcpp::Rcerr << exc.what() << "\n";
        return R_NilValue;
    }
}

RcppExport void rnetcore_export_ncall(SEXP arg_domain, SEXP arg_assembly, SEXP arg_type, SEXP arg_method)
{
    clr_app_domain *domain = Rcpp::XPtr<clr_app_domain>(arg_domain);
    const char *assembly = Rcpp::CharacterVector(arg_assembly)[0];
    const char *type = Rcpp::CharacterVector(arg_type)[0];
    const char *method = Rcpp::CharacterVector(arg_method)[0];
    
    utf16string assembly_u16 = utf8_to_utf16(assembly);
    utf16string type_u16 = utf8_to_utf16(type);
    utf16string method_u16 = utf8_to_utf16(method);

    void (*delegate)();
    try
    {
        delegate = (void (*)())domain->create_delegate(assembly_u16.c_str(), type_u16.c_str(), method_u16.c_str());
    }
    catch (const std::exception &exc)
    {
        Rcpp::Rcerr << "unable to obtain delegate.\n";
        Rcpp::Rcerr << exc.what() << "\n";
        return;
    }

    delegate();
}
