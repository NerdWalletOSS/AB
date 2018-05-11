-- Used to escape "'s by toCSV
local CSV = {}
function CSV.escape(s)
    if string.find(s, '[,"]') then
        s = '"' .. string.gsub(s, '"', '""') .. '"'
    end
    return s
end

-- Convert from CSV string to table (converts a single line of a CSV file)
function CSV.csv_to_table(s)
    s = s .. ','        -- ending comma
    local t = {}        -- table to collect fields
    local fieldstart = 1
    repeat
        -- next field is quoted? (start with `"'?)
        if string.find(s, '^"', fieldstart) then
            local a, c
            local i  = fieldstart
            repeat
                -- find closing quote
                a, i, c = string.find(s, '"("?)', i+1)
            until c ~= '"'    -- quote not followed by quote?
            if not i then error('unmatched "') end
            local f = string.sub(s, fieldstart+1, i-1)
            table.insert(t, (string.gsub(f, '""', '"')))
            fieldstart = string.find(s, ',', i) + 1
        else                -- unquoted; find next comma
            local nexti = string.find(s, ',', fieldstart)
            table.insert(t, string.sub(s, fieldstart, nexti-1))
            fieldstart = nexti + 1
        end
    until fieldstart > string.len(s)
    return t
end

-- Convert from table to CSV string
function CSV.table_to_csv (tt)
    local s = ""
    -- ChM 23.02.2014: changed pairs to ipairs
    -- assumption is that fromCSV and toCSV maintain data as ordered array
    for _,p in ipairs(tt) do
        s = s .. "," .. CSV.escape_csv(p)
    end
    return string.sub(s, 2)      -- remove first comma
end

function CSV.csv_string_to_list(str, headers)
    headers = headers or false
    local entry_size = nil
    local t = {}
    if headers then
        header_list = nil
        for line in string.gmatch(str, "[^\n]+") do
            if string.len(line:gsub("%s+", "")) > 0 then
                if header_list == nil then
                    header_list = CSV.csv_to_table(line)
                    entry_size = #header_list
                else
                    local entry = CSV.csv_to_table(line)
                    assert(#entry == entry_size, "Entries should have the same number of elements")
                    local t_entry = {}
                    for i=1,#entry do
                        t_entry[header_list[i]] = entry[i]
                    end
                    t[#t + 1] = t_entry
                end
            end
        end
    else
        for line in string.gmatch(str, ".*$") do
            if string.len(line:gsub("%s+", "")) > 0 then
                local t_entry = CSV.csv_to_table(line)
                entry_size = entry_size or #t_entry
                assert(#t_entry == entry_size, "Entries should have the same number of elements")
                t[#t + 1] = t_entry
            end
        end
    end
    return t
end

function CSV.csv_file_to_list(filepath, headers)
    headers = headers or false
    local entry_size = nil
    local t = {}
    if headers then
        header_list = nil
        for line in io.lines(filepath) do
            if header_list == nil then
                header_list = CSV.csv_to_table(line)
                entry_size = #header_list
            else
                local entry = CSV.csv_to_table(line)
                assert(#entry == entry_size, "Entries should have the same number of elements")
                local t_entry = {}
                for i=1,#entry do
                    t_entry[header_list[i]] = entry[i]
                end
                t[#t + 1] = t_entry
            end
        end
    else
        for line in io.lines(filepath) do
            local t_entry = CSV.csv_to_table(line)
            entry_size = entry_size or #t_entry
            assert(#t_entry == entry_size, "Entries should have the same number of elements")
            t[#t + 1] = t_entry
        end
    end
    return t
end

return CSV
