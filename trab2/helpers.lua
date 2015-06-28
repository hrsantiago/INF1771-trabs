function string.split(inputstr, sep)
  if sep == nil then
    sep = "%s"
  end

  local t = {}
  local i = 1
  for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
    t[i] = str
    i = i + 1
  end
  return t
end

function table.dump(t, depth, maxDepth)
  if not depth then depth = 0 end
  maxDepth = maxDepth or 3
  if depth > maxDepth then return end
  for k,v in pairs(t) do
    str = (' '):rep(depth * 2) .. k .. ': '
    if type(v) ~= "table" then
      print(str .. tostring(v))
    else
      print(str)
      table.dump(v, depth+1, maxDepth)
    end
  end
end

function table.removestr(t, str)
  for i=#t, 1, -1 do
    if string.find(t[i], str) then
      table.remove(t, i)
    end
  end
end

function table.removeemptystr(t)
  for i=#t, 1, -1 do
    if string.len(t[i]) == 0 then
      table.remove(t, i)
    end
  end
end

function table.replacestr(t, from, to)
  for key,str in ipairs(t) do
    t[key] = string.gsub(str, from, to)
  end
end

function table.lowerstr(t)
  for key,str in ipairs(t) do
    t[key] = string.lower(str)
  end
end
