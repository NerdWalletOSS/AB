#!/bin/bash
 auxwewf | grep "kafka" | grep -v "grep" | awk '{print $2}' | xargs kill -9