#!/bin/bash

# If any command ends with an error, terminate the entire script with an error.
set -e

##########################################################################
# Variables declaration                                                  #
##########################################################################

DEVICE="crok"

##########################################################################
# Logging                                                                #
##########################################################################

declare -A __severity_levels=([TRACE]=0 [DEBUG]=1 [INFO]=2 [WARN]=3 [ERROR]=4)
__severity_level="WARN"

function log_level { echo "${__severity_level}"; }
function logging_set_severity_level { if [[ ${__severity_levels[${1}]} ]]; then __severity_level="${1}"; fi; }

function log
{
    local log_lvl=$1
    local log_msg=$2

    # Check if level exists.
    if [[ ! ${__severity_levels[${log_lvl}]} ]]; then return; fi
    # Check if level is enough.
    if (( ${__severity_levels[${log_lvl}]} < ${__severity_levels[${__severity_level}]} )); then
        return
    fi

    echo "[${log_lvl}] ${log_msg}"
}

function log_trace { log "TRACE" "$1"; }
function log_debug { log "DEBUG" "$1"; }
function log_info  { log "INFO"  "$1"; }
function log_warn  { log "WARN"  "$1"; }
function log_error { log "ERROR" "$1"; }

##########################################################################
# Private functions                                                      #
##########################################################################

function dest_device { echo "/dev/${DEVICE}"; }
function get_repository_root_dir { echo "$( realpath "$( dirname "$( realpath "$0" )" )" )/.."; }
function major_version { echo "$( awk "\$2==\"$DEVICE\" {print \$1}" /proc/devices )"; }
function module_source_dir { echo "$( get_repository_root_dir )/build_release/drivers/${DEVICE}"; }
function module_source_file { echo "$( module_source_dir )/${DEVICE}.ko"; }

function create_node
{
    local major_ver="$1"
    local dest_device="$( dest_device )"
    mknod "${dest_device}" c "${major_ver}" 0
    local rc="$?"

    if [[ "$rc" -eq "0" ]]; then
        chmod o+w ${dest_device}
        chmod o+r ${dest_device}
    fi
    echo $rc
}

function remove_node
{
    local dest_device="$( dest_device )"
    log_debug "Destination device: '${dest_device}'"
    if [[ ! -e "${dest_device}" ]]; then
        log_debug "Device '"${dest_device}"' does not exist"
        return
    fi

    rm -f "${dest_device}"
}

##########################################################################
# Public functions                                                       #
##########################################################################

function load_device
{
	local dev_path="$( module_source_file )"

    insmod "${dev_path}"
    local rc="$?"
    if [[ "$rc" != "0" ]]; then
        log_error "Failed to install ${DEVICE}"
        exit 1
    fi

    local major_ver=$( major_version )
    log_debug "Major version '${major_ver}'"
    if [[ "${major_ver}" -eq "" ]]; then
        log_error "Invalid major version."
        exit 1
    fi

    remove_node
    rc=$( create_node ${major_ver} )
    if [[ "$rc" != "0" ]]; then
        log_error "Failed to create device node"
        rmmod "${DEVICE}"
        exit 1
    fi
}

function unload_device
{
    rmmod "${DEVICE}"
    remove_node
}

function usage
{
    echo "Usage: $0 {--load|--unload|--reload}"
    echo ""
    echo "Commands:"
    echo "  -l|--load   - load device."
    echo "  -r|--reload - reload device."
    echo "  -u|--unload - unload device."
    echo "Arguments:"
    echo "  --loglevel  - log level (TRACE, DEBUG, INFO, WARN or ERROR). Default 'WARN'."
    echo "  -h|--help   - show this message."
}

##########################################################################
# Main                                                                   #
##########################################################################

__is_load=0
__is_reload=0
__is_unload=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        -l|--load)
            __is_load=1
            shift   # pass arg
            ;;
        -r|--reload)
            __is_reload=1
            shift   # pass arg
            ;;
        -u|--unload)
            __is_unload=1
            shift   # pass arg
            ;;
        --loglevel)
            logging_set_severity_level "$2"
            shift # past arg
            shift # past val
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -*|--*)
            echo "Unknown option '"$1"'"
            usage
            exit 1
            ;;
    esac
done

if [[ "$(($__is_load + $__is_reload + $__is_unload))" -ne "1" ]]; then
    echo "Invalid input parameters. The command must be one."
    echo ""
    usage
    exit 1
fi

if [[ "$__is_load" -eq "1" ]]; then
    if [[ ! -f "$( module_source_file )" ]]; then
        echo "Module does not exist ("$( module_source_file )")."
        exit 1
    fi
    log_info "Load device ${DEVICE}"
    load_device
    exit 0
elif [[ "$__is_reload" -eq "1" ]]; then
    if [[ ! -f "$( module_source_file )" ]]; then
        echo "Module does not exist ("$( module_source_file )")."
        exit 1
    fi
    log_info "Reload device ${DEVICE}"
    unload_device
    load_device
    exit 0
elif [[ "$__is_unload" -eq "1" ]]; then
    log_info "Unload device ${DEVICE}"
    unload_device
    exit 0
fi

