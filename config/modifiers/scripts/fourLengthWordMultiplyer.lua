function OnTrigger(context)
    local points = 0
    for index, value in ipairs(context.words) do
        if #value == 4 then
            points = points + 10
        end
    end

    return points
end