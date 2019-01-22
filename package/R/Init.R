
.nwhich <- function() {
    return(.Call("rnetcore_export_nwhich", PACKAGE = "rNetCore"))
}

.nversions <- function(directory) {
    return(.Call("rnetcore_export_nversions", directory, PACKAGE = "rNetCore"))
}

.nload <- function(directory, version) {
    return(.Call("rnetcore_export_nload_coreclr", directory, version, PACKAGE = "rNetCore"))
}

.ninit <- function(coreclr, domain) {
    return(.Call("rnetcore_export_ninit_app_domain", coreclr, domain, PACKAGE = "rNetCore"))
}

.ncall <- function(domain, assembly, type, method) {
    .Call("rnetcore_export_ncall", domain, assembly, type, method, PACKAGE = "rNetCore")
}
