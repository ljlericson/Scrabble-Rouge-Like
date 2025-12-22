return function(context)
    local points = 0

    for _, word in ipairs(context.words) do
        if #word == 4 then
            points = points + 50
        end
    end

    return {
        addScore = points
    }
end